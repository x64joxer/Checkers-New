#include "Worker.h"

Worker::Worker() : connection_state(DISCONNECTED),
                   myState(Peers::STATE::FREE),
                   maxThread(ProgramVariables::GetMaxThredForIa()),
                   endIaJobFlag(false),
                   conversationIsOngoing(false),
                   stopFlag(false),
                   canNotMoveFlag(false)
{
    TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: Worker::Worker()";

    messageQueue = new SharedPtrList<Message>;

    if (ProgramVariables::GetTrafficFlag())
    {
        jobExpander = nullptr;
    } else
    {
        jobExpander = new ThreadIAMove<3000000>();
        jobExpander->GetThreadIABoardQueueWsk()->SetConditionVariable(condition_var);
    }
}

void Worker::Start()
{
    TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: void Worker::Start()";

    socketToServer.SetMessageQueue(messageQueue);
    socketToServer.Connect(ProgramVariables::GetIpForScheduler(), ProgramVariables::GetPortForScheduler());

    workerThread = std::move(std::thread(&Worker::StartWorking,
                                          this));

    workerThread.join();
}

void Worker::StartWorking()
{
    TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: void Worker::StartWorking()";

    condition_var = messageQueue->GetCondVar();
    ThreadIABoardQueue<3000000> * boardThradWsk;

    if (jobExpander)
    {
        boardThradWsk = jobExpander->GetThreadIABoardQueueWsk();
        boardThradWsk->SetConditionVariable(condition_var);
    }

    char *dest = new char[MessageCoder::MaxMessageSize()];

    Message tmpMessage;

    QueueTimer reconnectionTimer;
    Message timeoutMessage;

    char *tmpChar = new char[MessageCoder::MaxMessageTimeoutSize()];
    MessageCoder::ClearChar(tmpChar, MessageCoder::MaxMessageTimeoutSize());
    MessageCoder::CreateTimeoutMessage(tmpChar);
    timeoutMessage.CopyWsk(socketToServer.GeetMyWsk(), tmpChar);

    reconnectionTimer.SetTime(ProgramVariables::GetMaxTimeoutForMessageResponse());
    reconnectionTimer.SetMessageToSend(timeoutMessage);
    reconnectionTimer.SetQueue(messageQueue);
    reconnectionTimer.Start();

    bool popFrontException = false;    
    bool jobToSendFast = false;
    Board tmpBoard;

    while(true)
    {
        TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: Waiting for a job..";
        std::mutex tmpMutex;
        std::unique_lock<std::mutex> guard(tmpMutex);

        condition_var->wait(guard,[this, &jobToSendFast, &tmpBoard]
        {
            if ((numOfResultToReturnFast > 0)&&(canITakeBoardToReturnFast)&&(!conversationIsOngoing))
            {
                if (jobExpander)
                {
                    tmpBoard = jobExpander->GetThreadIABoardQueueWsk()->PopFront(0);
                    jobToSendFast = !tmpBoard.GetNullBoard();
                }
            }

            return (!messageQueue->Empty()) | (endIaJobFlag&&(!conversationIsOngoing)) | jobToSendFast | canNotMoveFlag;
        }
        );

        try
        {
            tmpMessage = messageQueue->PopFront();
        }
        catch (...)
        {
            TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: Pop front exception";

            popFrontException = true;
        }

        if (!popFrontException)
        {
            std::map<std::string, std::string> messageContent;
            MessageCoder::MessageToMap(tmpMessage.GetWskMessage(), messageContent);

            MessageInterpreting(tmpMessage.GetTCPSocket_ptr(), messageContent, dest, reconnectionTimer, prevousMessageid);
        } else
        if (endIaJobFlag)
        {
            SendBestResultWhenJobEnd(boardToAnalyse, dest, jobId, reconnectionTimer);
        } else
        if (jobToSendFast)
        {
            jobToSendFast = false;
            if (numOfResultToReturnFast > 0) numOfResultToReturnFast--;
            SendResult(tmpBoard, dest, jobId, reconnectionTimer);
        } else
        if (canNotMoveFlag)
        {
            canNotMoveFlag = false;
            SendCanNotMoveMessage(dest, jobId, reconnectionTimer);
        }

        popFrontException = false;
    }

    delete [] dest;
}

void Worker::MessageInterpreting(TCPSocket_ptr socket, std::map<std::string, std::string> & data, char * dest, QueueTimer & reconnectionTimer, std::string & prevousMessageid)
{
    TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: void Worker::MessageInterpreting(TCPSocket_ptr socket, std::map<std::string, std::string> & data, char * dest, QueueTimer & reconnectionTimer, std::string & prevousMessageid)";

    try
    {
        std::string action = data.at(MessageCoder::ACTION);

        if (action == MessageCoder::OK)
        {
            TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: Message OK received";
            conversationIsOngoing = false;

            if (connection_state == CONNECTED)
            {                
                reconnectionTimer.Stop();

                 if (prevousMessageid == data.at(MessageCoder::MESSAGE_ID))
                 {
                     connection_state = ConState::REGISTERED;                                          

                     SendStateMessage(socket, dest, prevousMessageid);
                     reconnectionTimer.Start();
                 } else
                 {
                    Traces() << "\n" << "ERR: Wrong message ID!";

                    SendRegisterMessage(socket, dest, prevousMessageid);
                    reconnectionTimer.Start();
                 }
            } else                
            if (connection_state == REGISTERED)
            {
                 reconnectionTimer.Stop();

                 if (prevousMessageid == data.at(MessageCoder::MESSAGE_ID))
                 {
                     connection_state = ConState::STATEUPDATED;
                 } else
                 {
                    Traces() << "\n" << "ERR: Wrong message ID!";

                    SendStateMessage(socket, dest, prevousMessageid);
                    reconnectionTimer.Start();
                 }
            } else
            if (connection_state == ConState::BEST_RESULT_SEND)
            {
                reconnectionTimer.Stop();

                if (prevousMessageid == data.at(MessageCoder::MESSAGE_ID))
                {
                    connection_state = ConState::REGISTERED;

                    SendStateMessage(socket, dest, prevousMessageid);
                    reconnectionTimer.Start();
                } else
                {
                    Traces() << "\n" << "ERR: Wrong message ID!";

                    connection_state = DISCONNECTED;
                    socket.get()->Connect(ProgramVariables::GetIpForScheduler(), ProgramVariables::GetPortForScheduler());
                    reconnectionTimer.Start();
                }
            } else
            if (connection_state == ConState::BEST_RESULT_FAST_SEND)
            {
                reconnectionTimer.Stop();

                if (prevousMessageid == data.at(MessageCoder::MESSAGE_ID))
                {
                    connection_state = ConState::STATEUPDATED;
                } else
                {
                    Traces() << "\n" << "ERR: Wrong message ID!";

                    connection_state = DISCONNECTED;
                    socket.get()->Connect(ProgramVariables::GetIpForScheduler(), ProgramVariables::GetPortForScheduler());
                    reconnectionTimer.Start();
                }
            } else
            if (connection_state == ConState::CAN_NOT_MOVE_SEND)
            {
                reconnectionTimer.Stop();

                if (prevousMessageid == data.at(MessageCoder::MESSAGE_ID))
                {
                    connection_state = ConState::REGISTERED;

                    SendStateMessage(socket, dest, prevousMessageid);
                    reconnectionTimer.Start();
                } else
                {
                    Traces() << "\n" << "ERR: Wrong message ID!";

                    connection_state = DISCONNECTED;
                    socket.get()->Connect(ProgramVariables::GetIpForScheduler(), ProgramVariables::GetPortForScheduler());
                    reconnectionTimer.Start();
                }
            } else
            {
                Traces() << "\n" << "ERR: Wrong connection state";
            }
        } else
        if (action == MessageCoder::CONNECTED)
        {
            TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: action == MessageCoder::CONNECTED";
            connection_state = CONNECTED;
            reconnectionTimer.Start();
            SendRegisterMessage(socket, dest, prevousMessageid);

        } else
        if (action == MessageCoder::CLOSE_CNNECTION)
        {
            TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: action == MessageCoder::CLOSE_CNNECTION";
            connection_state = DISCONNECTED;
            reconnectionTimer.Start();

        } else
        if (action == MessageCoder::START_ANALYSE)
        {
            TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: action == MessageCoder::START_ANALYSE";
            ReceiveJob(socket, data, dest, reconnectionTimer);

        } else
        if (action == MessageCoder::STOP_ANALYSE)
        {
            TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: action == MessageCoder::STOP_ANALYSE";
            ReceiveStopAnalyse(socket, data, dest);

        } else
        if (action == MessageCoder::START_ANALYSE_FAST)
        {
            TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: action == MessageCoder::START_ANALYSE_FAST";
            ReceiveJob(socket, data, dest, reconnectionTimer, true);

        } else            
        if (action == MessageCoder::TIMEOUT)
        {
            TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: action == MessageCoder::TIMEOUT";

            if (connection_state == DISCONNECTED)
            {
                socket.get()->Connect(ProgramVariables::GetIpForScheduler(), ProgramVariables::GetPortForScheduler());
                reconnectionTimer.Start();
            } else
            {
                socket.get()->Close();
            }
        } else
        {
            Traces() << "\n" << "ERR: Unexpected action: " << action;
        }

    }
    catch (std::out_of_range)
    {
        Traces() << "\n" << "ERR: Protocol error";
    }
}

void Worker::SendRegisterMessage(TCPSocket_ptr socket, char * dest, std::string & prevousMessageid)
{
    TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: void Worker::SendRegisterMessage()";

    MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
    prevousMessageid = MessageCoder::CreateMessageId();
    MessageCoder::CreateRoleMessage(MessageCoder::ROLE_ENUM::WORKER, prevousMessageid, dest);
    conversationIsOngoing = true;
    socket->WriteMessage(dest);
}

void Worker::SendStateMessage(TCPSocket_ptr socket, char * dest, std::string & prevousMessageid)
{
    TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: void Worker::SendStateMessage(TCPSocket_ptr socket, char * dest, std::string & prevousMessageid)";

    conversationIsOngoing = true;
    MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
    prevousMessageid = MessageCoder::CreateMessageId();

    MessageCoder::CreateStateMessage(myState, maxThread, prevousMessageid, dest);
    socket->WriteMessage(dest);
}

void Worker::SendBestResultWhenJobEnd(Board & board, char * dest, std::string & jobId, QueueTimer & reconnectionTimer)
{
    TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: void Worker::SendBestResultWhenJobEnd(Board & board, char * dest, std::string & jobId, QueueTimer & reconnectionTimer)";

    endIaJobFlag = false;
    myState = Peers::STATE::FREE;    
    connection_state = ConState::BEST_RESULT_SEND;
    conversationIsOngoing = true;

    Counters::AddToCounterNumberOfBestResultMessagesSend(1);

    /////////////////////////
    //Traffic test purposes
    /////////////////////////
    if (previousTimeOfClearCounternumberOfBestResultMessagesSend == 0)
    {
        previousTimeOfClearCounternumberOfBestResultMessagesSend = Traces::GetMilisecondsSinceEpoch();
    } else
    if (previousTimeOfClearCounternumberOfBestResultMessagesSend + 30000 < Traces::GetMilisecondsSinceEpoch())
    {
        Traces() << "\n" << "LOG: Counter numberOfBestResultMessagesSend: " << Counters::GetCounterNumberOfBestResultMessagesSend() / 30 << " m/s";
        Counters::ClearCounterNumberOfBestResultMessagesSend();
        previousTimeOfClearCounternumberOfBestResultMessagesSend = Traces::GetMilisecondsSinceEpoch();
    }
    /////////////////////////
    //End Traffic test purposes
    /////////////////////////

    MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
    prevousMessageid = MessageCoder::CreateMessageId();
    MessageCoder::CreateBestResultMessage(board, prevousMessageid, jobId, Counters::GetCounterNumberOfAnalysedBoard(), dest);
    socketToServer.WriteMessage(dest);
    reconnectionTimer.Start();
}

void Worker::SendResult(Board & board, char * dest, std::string & jobId, QueueTimer & reconnectionTimer)
{
    TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: void Worker::SendResult(Board & board, char * dest, std::string & prevousMessageid, std::string & jobId, QueueTimer & reconnectionTimer)";

    connection_state = ConState::BEST_RESULT_FAST_SEND;
    conversationIsOngoing = true;

    MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
    prevousMessageid = MessageCoder::CreateMessageId();
    MessageCoder::CreateBestResultMessage(board, prevousMessageid, jobId, 1, dest);
    socketToServer.WriteMessage(dest);
    reconnectionTimer.Start();
}

void Worker::SendCanNotMoveMessage(char * dest, std::string & jobId, QueueTimer & reconnectionTimer)
{
    TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: void SendCanNotMoveMessage(TCPSocket_ptr socket, char * dest, std::string & jobId, QueueTimer & reconnectionTimer)";

    endIaJobFlag = false;
    myState = Peers::STATE::FREE;
    connection_state = ConState::CAN_NOT_MOVE_SEND;
    conversationIsOngoing = true;

    MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
    prevousMessageid = MessageCoder::CreateMessageId();
    MessageCoder::CreateCanNotMoveMessage(dest, jobId);

    socketToServer.WriteMessage(dest);
    reconnectionTimer.Start();
}

void Worker::ReceiveStopAnalyse(TCPSocket_ptr socket, std::map<std::string, std::string> & data, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: void Worker::ReceiveStopAnalyse(TCPSocket_ptr socket, std::map<std::string, std::string> & data, char * dest)";

    stopFlag = true;

    while (endIaJobFlag) {}
    myState = Peers::STATE::FREE;

    MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
    MessageCoder::CreateOkMessage(data.at(MessageCoder::MESSAGE_ID), dest);
    socket->WriteMessage(dest);

}

void Worker::ReceiveJob(TCPSocket_ptr socket, std::map<std::string, std::string> & data, char * dest, QueueTimer & reconnectionTimer, bool fast)
{
    TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: void Worker::ReceiveJob(TCPSocket_ptr socket, std::map<std::string, std::string> & data, char * dest, QueueTimer & reconnectionTimer, std::string & prevousMessageid)";

    /////////////////////////
    //Traffic test purposes
    /////////////////////////
    if (ProgramVariables::GetTrafficFlag())
    {
        std::string messageId = data.at(MessageCoder::MESSAGE_ID);
        jobId = data.at(MessageCoder::JOB_ID);
        MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
        MessageCoder::CreateOkMessage(messageId, dest);
        socket->WriteMessage(dest);

        std::this_thread::sleep_for(std::chrono::milliseconds(ProgramVariables::GetDelayBetweenBestResultResponses()));

        Board tmpBoard;
        SendBestResultWhenJobEnd(tmpBoard, destTraffic, jobId, reconnectionTimer);
    } else
    /////////////////////////
    //END traffic test purposes
    /////////////////////////
    if (myState == Peers::STATE::FREE)
    {
        TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: Receiving data from start analyse message";

        Counters::ClearCounterNumberOfAnalysedBoard();
        endIaJobFlag = false;
        stopFlag = false;
        canNotMoveFlag = false;
        canITakeBoardToReturnFast = false;
        maxIaTime = std::atoi(data.at(MessageCoder::MAX_TIME).c_str());
        if (fast)
        {
            numOfResultToReturnFast = std::atoi(data.at(MessageCoder::NUM_OF_BOARD_TO_RETURN_FAST).c_str());
        } else
        {
            numOfResultToReturnFast = 0;
        }
        MessageCoder::MapToBoard(data, &boardToAnalyse);
        jobId = data.at(MessageCoder::JOB_ID);
        firstWorker = std::atoi(data.at(MessageCoder::IS_FIRST_WORKER).c_str());

        std::thread tempJob(&ThreadIAMove<3000000>::operator (),
                            jobExpander,
                            &boardToAnalyse,
                            &endIaJobFlag,
                            &stopFlag,
                            &currentPercentOfSteps,
                            maxThread,
                            3000,
                            CalculateMaxTimeForIA(maxIaTime, ProgramVariables::GetTimeReserveToSendBestResultToScheduler()),
                            KindOfSteps::Time,
                            firstWorker,
                            &canITakeBoardToReturnFast,
                            numOfResultToReturnFast,
                            &canNotMoveFlag);

        tempJob.detach();
        iaJob = std::move(tempJob);

        TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: Sending OK message";

        std::string messageId = data.at(MessageCoder::MESSAGE_ID);
        MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
        MessageCoder::CreateOkMessage(messageId, dest);
        socket->WriteMessage(dest);                

    } else
    if (myState == Peers::STATE::BUSY)
    {
        std::string messageId = data.at(MessageCoder::MESSAGE_ID);
        MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
        MessageCoder::CreateNotOkMessage(messageId, MessageCoder::NOT_OK_REASON::BUSY, dest);
        socket->WriteMessage(dest);
    }
}

unsigned int Worker::CalculateMaxTimeForIA(const unsigned int maxTime, const unsigned int reservedTime)
{
    TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: unsigned int Worker::CalculateMaxTimeForIA(const unsigned int maxTime, const unsigned int reservedTime)";

    if (maxTime > reservedTime) return maxTime - reservedTime;
    return maxTime;
}

Worker::~Worker()
{
    TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: Worker::~Worker()";
    delete messageQueue;
    delete [] destTraffic;
}
