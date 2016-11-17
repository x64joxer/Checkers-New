#include "MessageCoder.h"

MessageCoder::MessageCoder()
{

}

void MessageCoder::KeyValuePairToChar(const std::string & key, const std::string & value, char *dest)
{
        int len = strlen(dest);       

        dest[len] = '<';
        dest[len+1] = 0;

        std::copy(key.begin(), key.end(), dest + strlen(dest));
        dest[strlen(dest)] = 0;

        len = strlen(dest);
        dest[len] = '>';       
        dest[len+1] = '<';
        dest[len+2] = 0;
        len = len +2;
        std::copy(value.begin(), value.end(), dest + strlen(dest));
        len = strlen(dest);
        dest[len] = '>';
        dest[len+1] = 0;
}

void MessageCoder::KeyValuePairToChar(const std::string & key, const int value, char *dest)
{
    KeyValuePairToChar(key, std::to_string(value), dest);
}

void MessageCoder::KeyValuePairToChar(const std::string & key, const unsigned int value, char *dest)
{
    KeyValuePairToChar(key, std::to_string(value), dest);
}

void MessageCoder::KeyValuePairToChar(const std::string & key, const short value, char *dest)
{
    KeyValuePairToChar(key, std::to_string(value), dest);
}

void MessageCoder::KeyValuePairToChar(const std::string & key, const long value, char *dest)
{
    KeyValuePairToChar(key, std::to_string(value), dest);
}

void MessageCoder::KeyValuePairToChar(const std::string & key, const unsigned short value, char *dest)
{
    KeyValuePairToChar(key, std::to_string(value), dest);
}

void MessageCoder::KeyValuePairToChar(const std::string & key, const unsigned long long value, char *dest)
{
    KeyValuePairToChar(key, std::to_string(value), dest);
}

void MessageCoder::KeyValuePairToChar(const std::string & key, const bool value, char *dest)
{
    KeyValuePairToChar(key, std::to_string(value), dest);
}

void MessageCoder::ClearChar(char *dest, const unsigned int num)
{
    for (unsigned int i=0;i<num;i++) dest[i] = 0;
}

void MessageCoder::InsertHeader(char *dest)
{
    for (unsigned int i=0;i<MessageCoder::BufferSize();i++) dest[i] = 65;
}


void MessageCoder::InsertLenMessageHeader(char *dest)
{
    unsigned int size = strlen(dest) - BufferSize();

    dest[0] = (size & 0x000F) + 65;
    dest[1] = ((size & 0x00F0) >> 4) + 65;
    dest[2] = ((size & 0x0F00) >> 8) + 65;
    dest[3] = ((size & 0xF000) >> 12) + 65;
}

void MessageCoder::InsertLenMessageHeader(const unsigned int val, char *dest)
{
    dest[0] = (val & 0x000F) + 65;
    dest[1] = ((val & 0x00F0) >> 4) + 65;
    dest[2] = ((val & 0x0F00) >> 8) + 65;
    dest[3] = ((val & 0xF000) >> 12) + 65;
}

unsigned int MessageCoder::HeaderToVal(char *dest)
{
     unsigned int result;
     result = (dest[0] - 65);
     result += (dest[1] - 65) << 4;
     result += (dest[2] - 65) << 8;
     result += (dest[3] - 65) << 12;

     return result;
}

void MessageCoder::MessageToMap(const char *source, std::map<std::string, std::string> & dest)
{    
    unsigned int i = 0;
    unsigned int begin;
    unsigned int end;
    unsigned int begin_val;
    unsigned int end_val;
    std::string keyString;
    bool key = false;

    dest.clear();

    while (source[i] != 0)
    {
        if (!key)
        {
            if (source[i] == '<') begin = i;
            if (source[i] == '>')
            {
                end = i;
                key = true;
            };
        } else
        {
            if (source[i] == '<') begin_val = i;
            if (source[i] == '>')
            {                
                end_val = i;
                key = false;
                keyString = std::string(source + begin + 1, source + end);                
                dest[keyString] = std::string(source + begin_val + 1, source + end_val);
            };
        }

        if (keyString == MESSAGE_END) break;

        i++;
    };

}

void MessageCoder::BoardToChar(const Board &board, char *dest, const unsigned short numberOfBoard)
{
     TRACE_FLAG_FOR_CLASS_MessageCoder Traces() << "\n" << "LOG: MessageCoder::BoardToChar(const Board &board, char *dest, const unsigned short numberOfBoard)";

    KeyValuePairToChar(PREVIOUS_MURDER, board.GetPreviousMurder(), dest);
    KeyValuePairToChar(WHITE_PATCH_END, board.GetWhitePatchEnd(), dest);
    KeyValuePairToChar(BLACK_WHITE, board.Black(), dest);

    //Origin

    Board temp = board.GetOrigin();
    std::string prefix = std::to_string(numberOfBoard) + "_";
    PawnPos pawnPos;
    unsigned short num;

    num = temp.GetNumberOfBlack();
    KeyValuePairToChar(prefix + ORIGIN_NUMBER_BLACK, num, dest);

    if (num > 0)
    {
        for (unsigned short i = 0; i<num; i++)
        {
            pawnPos = temp.GetBlackPawnPos(i);
            KeyValuePairToChar(prefix + ORIGIN_BLACK_X + std::to_string(i), pawnPos.X(), dest);
            KeyValuePairToChar(prefix + ORIGIN_BLACK_Y + std::to_string(i), pawnPos.Y(), dest);
            KeyValuePairToChar(prefix + ORIGIN_BLACK_PONS + std::to_string(i), temp.GetBlackPawnPons(i), dest);
        }
    }

    num = temp.GetNumberOfWhite();
    KeyValuePairToChar(prefix + ORIGIN_NUMBER_WHITE, num, dest);

    if (num > 0)
    {
        for (unsigned short i = 0; i<num; i++)
        {
            pawnPos = temp.GetWhitePawnPos(i);
            KeyValuePairToChar(prefix + ORIGIN_WHITE_X + std::to_string(i), pawnPos.X(), dest);
            KeyValuePairToChar(prefix + ORIGIN_WHITE_Y + std::to_string(i), pawnPos.Y(), dest);
            KeyValuePairToChar(prefix + ORIGIN_WHITE_PONS + std::to_string(i), temp.GetWhitePawnPons(i), dest);
        }
    }


    //Board

    num = board.GetNumberOfBlack();
    KeyValuePairToChar(prefix + NUMBER_BLACK, num, dest);

    TRACE_FLAG_FOR_CLASS_MessageCoder board.PrintDebug();

    if (num > 0)
    {
        for (unsigned short i = 0; i<num; i++)
        {            
            pawnPos = board.GetBlackPawnPos(i);
            KeyValuePairToChar(prefix + BLACK_X + std::to_string(i), pawnPos.X(), dest);            
            KeyValuePairToChar(prefix + BLACK_Y + std::to_string(i), pawnPos.Y(), dest);           
            KeyValuePairToChar(prefix + BLACK_PONS + std::to_string(i), board.GetBlackPawnPons(i), dest);
        }
    }

    num = board.GetNumberOfWhite();
    KeyValuePairToChar(prefix + NUMBER_WHITE, num, dest);

    if (num > 0)
    {
        for (unsigned short i = 0; i<num; i++)
        {
            pawnPos = board.GetWhitePawnPos(i);
            KeyValuePairToChar(prefix + WHITE_X + std::to_string(i), pawnPos.X(), dest);
            KeyValuePairToChar(prefix + WHITE_Y + std::to_string(i), pawnPos.Y(), dest);
            KeyValuePairToChar(prefix + WHITE_PONS + std::to_string(i), board.GetWhitePawnPons(i), dest);
        }
    }

    KeyValuePairToChar(MESSAGE_END, 0, dest);
}

void MessageCoder::MapToBoard(const std::map<std::string, std::string> & dest, Board *board)
{
    try
    {
        board->Clear();

        board->SetPreviousMurder(atoi(dest.at(PREVIOUS_MURDER).c_str()));
        board->SetWhitePatchEnd(atoi(dest.at(WHITE_PATCH_END).c_str()));

        if (atoi(dest.at(BLACK_WHITE).c_str()) == 0)
        {
            board->StartBlack();
        } else
        {
            board->StartWhite();
        }

        Board origin;
        std::string prefix = "1_";

        std::string temp;
        std::string tempX;
        std::string tempY;
        std::string tempPons;

        temp = dest.at(prefix + ORIGIN_NUMBER_BLACK);
        unsigned short numBlack = atoi(temp.c_str());
        temp = dest.at(prefix + ORIGIN_NUMBER_WHITE);
        unsigned short numWhite = atoi(temp.c_str());

        if (numBlack > 0)
        {
            for (unsigned short i = 0; i<numBlack; i++)
            {
                tempX = dest.at(prefix + ORIGIN_BLACK_X + std::to_string(i));
                tempY = dest.at(prefix + ORIGIN_BLACK_Y + std::to_string(i));
                tempPons = dest.at(prefix + ORIGIN_BLACK_PONS + std::to_string(i));

                origin.AddBlackPawn(atoi(tempX.c_str()),
                                    atoi(tempY.c_str()),
                                    0,
                                    atoi(tempPons.c_str()));
            }
        }


        if (numWhite > 0)
        {
            for (unsigned short i = 0; i<numWhite; i++)
            {
                tempX = dest.at(prefix + ORIGIN_WHITE_X + std::to_string(i));
                tempY = dest.at(prefix + ORIGIN_WHITE_Y + std::to_string(i));
                tempPons = dest.at(prefix + ORIGIN_WHITE_PONS + std::to_string(i));

                origin.AddWhitePawn(atoi(tempX.c_str()),
                                    atoi(tempY.c_str()),
                                    0,
                                    atoi(tempPons.c_str()));
            }
        }

        board->SetOrigin(origin);

        temp = dest.at(prefix + NUMBER_BLACK);
        numBlack = atoi(temp.c_str());
        temp = dest.at(prefix + NUMBER_WHITE);
        numWhite = atoi(temp.c_str());

        if (numBlack > 0)
        {
            for (unsigned short i = 0; i<numBlack; i++)
            {
                tempX = dest.at(prefix + BLACK_X + std::to_string(i));
                tempY = dest.at(prefix + BLACK_Y + std::to_string(i));
                tempPons = dest.at(prefix + BLACK_PONS + std::to_string(i));

                board->AddBlackPawn(atoi(tempX.c_str()),
                                    atoi(tempY.c_str()),
                                    0,
                                    atoi(tempPons.c_str()));
            }
        }

        if (numWhite > 0)
        {
            for (unsigned short i = 0; i<numWhite; i++)
            {
                tempX = dest.at(prefix + WHITE_X + std::to_string(i));
                tempY = dest.at(prefix + WHITE_Y + std::to_string(i));
                tempPons = dest.at(prefix + WHITE_PONS + std::to_string(i));

                board->AddWhitePawn(atoi(tempX.c_str()),
                                    atoi(tempY.c_str()),
                                    0,
                                    atoi(tempPons.c_str()));
            }
        }
    }
    catch (std::out_of_range)
    {
        throw;
    }
}

void MessageCoder::CreateConnectedMessage(char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, CONNECTED, dest);
    KeyValuePairToChar(MESSAGE_END, 0, dest);
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateCloseConnectionMessage(char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, CLOSE_CNNECTION, dest);
    KeyValuePairToChar(MESSAGE_END, 0, dest);    
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateTimeToSendResultToClientsMessage(char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, TIME_TO_SEND_RESULT_TO_CLIENTS, dest);
    KeyValuePairToChar(MESSAGE_END, 0, dest);
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateTimeoutMessage(char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, TIMEOUT, dest);
    KeyValuePairToChar(MESSAGE_END, 0, dest);
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateStartAnalyseWorkAndReturnNResultFast(const unsigned long long respTime, const unsigned int numOfResultToReturnFast, const Board & board, const std::string & id, const std::string & jobId, char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, START_ANALYSE_FAST, dest);
    KeyValuePairToChar(MAX_TIME, respTime, dest);
    KeyValuePairToChar(MESSAGE_ID, id, dest);    
    KeyValuePairToChar(JOB_ID, jobId, dest);    
    KeyValuePairToChar(IS_FIRST_WORKER, true, dest);
    KeyValuePairToChar(NUM_OF_BOARD_TO_RETURN_FAST, numOfResultToReturnFast, dest);
    MessageCoder::BoardToChar(board, dest, 1);
    KeyValuePairToChar(MESSAGE_END, 0, dest);
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateStartAnalyseWork(const unsigned long long respTime , const Board & board, const std::string & id, const std::string & jobId, const bool isFirstWorker, char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, START_ANALYSE, dest);
    KeyValuePairToChar(MAX_TIME, respTime, dest);
    KeyValuePairToChar(MESSAGE_ID, id, dest);
    KeyValuePairToChar(JOB_ID, jobId, dest);
    KeyValuePairToChar(IS_FIRST_WORKER, isFirstWorker, dest);
    MessageCoder::BoardToChar(board, dest, 1);
    KeyValuePairToChar(MESSAGE_END, 0, dest);
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateResetServerStateMessage(const Board & board, const std::string & id, char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, RESET_SERVER_STATE, dest);
    MessageCoder::BoardToChar(board, dest, 1);
    KeyValuePairToChar(MESSAGE_END, 0, dest);
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateStartMessage(const unsigned long long respTime, const unsigned short numberOfBoard, const std::string & id, const std::string & jobId, const Board & board, char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, START_WORK, dest);
    KeyValuePairToChar(MAX_TIME, respTime, dest);
    KeyValuePairToChar(MESSAGE_ID, id, dest);
    KeyValuePairToChar(JOB_ID, jobId, dest);
    KeyValuePairToChar(NUM_OF_BOARD, numberOfBoard, dest);
    MessageCoder::BoardToChar(board, dest, 1);
    KeyValuePairToChar(MESSAGE_END, 0, dest);
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateBestResultMessage(const Board & board, const std::string & id, const std::string & jobId, unsigned long long numOfAnalysed, char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, BEST_RESULT, dest);
    KeyValuePairToChar(NUM_OF_ANALYSED, numOfAnalysed, dest);
    KeyValuePairToChar(MESSAGE_ID, id, dest);
    KeyValuePairToChar(JOB_ID, jobId, dest);    
    MessageCoder::BoardToChar(board, dest, 1);
    KeyValuePairToChar(MESSAGE_END, 0, dest);
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateGetServerStateMessage(const std::string & id, char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, GET_SERVER_STATE, dest);
    KeyValuePairToChar(MESSAGE_ID, id, dest);
    KeyValuePairToChar(MESSAGE_END, 0, dest);
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateStateMessage(const Peers::STATE stat, const unsigned int numOfThread, const std::string & id, char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, SET_STATE, dest);
    KeyValuePairToChar(MESSAGE_ID, id, dest);
    KeyValuePairToChar(STATE, stat, dest);
    KeyValuePairToChar(NUM_OF_THREAD, numOfThread, dest);
    KeyValuePairToChar(MESSAGE_END, 0, dest);        
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateServerStateMessage(const ServerState & serverState, const std::string & id, char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, SERVER_STATE, dest);
    KeyValuePairToChar(MESSAGE_ID, id, dest);

    KeyValuePairToChar(IS_THINKING, serverState.IsThinking(), dest);
    KeyValuePairToChar(MAX_IA_TIME, serverState.GetMaxTime(), dest);
    KeyValuePairToChar(START_TIME, serverState.GetStartTime(), dest);
    KeyValuePairToChar(TIME_TO_END, serverState.GetTimeToEnd(), dest);
    BoardToChar(serverState.GetBoard(), dest + strlen(dest) , 1);

    KeyValuePairToChar(MESSAGE_END, 0, dest);
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateRoleMessage(const ROLE_ENUM role, const std::string & id, char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, SET_ROLE, dest);
    KeyValuePairToChar(MESSAGE_ID, id, dest);
    KeyValuePairToChar(ROLE, role, dest);
    KeyValuePairToChar(MESSAGE_END, 0, dest);
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateStopAnalyse(const std::string & id, char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, STOP_ANALYSE, dest);
    KeyValuePairToChar(MESSAGE_ID, id, dest);
    KeyValuePairToChar(MESSAGE_END, 0, dest);
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateOkMessage(const std::string  & id, char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, OK, dest);
    KeyValuePairToChar(MESSAGE_ID, id, dest);
    KeyValuePairToChar(MESSAGE_END, 0, dest);
    InsertLenMessageHeader(dest);
}

void MessageCoder::CreateNotOkMessage(const std::string & id, NOT_OK_REASON reason, char *dest)
{
    InsertHeader(dest);
    KeyValuePairToChar(ACTION, NOT_OK, dest);
    KeyValuePairToChar(REASON, int(reason), dest);
    KeyValuePairToChar(MESSAGE_ID, id, dest);
    KeyValuePairToChar(MESSAGE_END, 0, dest);
    InsertLenMessageHeader(dest);
}

std::string MessageCoder::GetNextKey(const std::string & debug_key)
{
    if (debugMode)
    {
        return debug_key;
    } else
    {
        nextKey++;
        return std::to_string(nextKey) + "_";
    }
}

std::string MessageCoder::CreateMessageId()
{
    std::lock_guard<std::mutex> guard(mutex_guard);
    return std::to_string(++messageId);    
}

std::string MessageCoder::ACTION = GetNextKey("ACTION");
std::string MessageCoder::OK = GetNextKey("OK");
std::string MessageCoder::NOT_OK = GetNextKey("NOT_OK");
std::string MessageCoder::SERVER_STATE = GetNextKey("SERVER_STATE");
std::string MessageCoder::MESSAGE_ID = GetNextKey("MESSAGE_ID");
std::string MessageCoder::MESSAGE_END = GetNextKey("MESSAGE_END");
std::string MessageCoder::CLOSE_CNNECTION = GetNextKey("CLOSE_CNNECTION");
std::string MessageCoder::CONNECTED = GetNextKey("CONNECTED");
std::string MessageCoder::START_ANALYSE = GetNextKey("START_ANALYSE");
std::string MessageCoder::START_ANALYSE_FAST = GetNextKey("START_ANALYSE_FAST");
std::string MessageCoder::TIMEOUT = GetNextKey("TIMEOUT");
std::string MessageCoder::TIME_TO_SEND_RESULT_TO_CLIENTS = GetNextKey("TIME_TO_SEND_RESULT_TO_CLIENTS");
std::string MessageCoder::RESET_SERVER_STATE = GetNextKey("RESET_SERVER_STATE");

std::string MessageCoder::STOP_ANALYSE = GetNextKey("STOP_ANALYSE");
std::string MessageCoder::START_WORK = GetNextKey("START_WORK");
std::string MessageCoder::SET_STATE = GetNextKey("SET_STATE");
std::string MessageCoder::SET_ROLE = GetNextKey("SET_ROLE");
std::string MessageCoder::GET_SERVER_STATE  = GetNextKey("GET_SERVER_STATE");
std::string MessageCoder::BEST_RESULT = GetNextKey("BEST_RESULT");

std::string MessageCoder::IS_THINKING = GetNextKey("IS_THINKING");
std::string MessageCoder::MAX_IA_TIME = GetNextKey("MAX_IA_TIME");
std::string MessageCoder::START_TIME = GetNextKey("START_TIME");
std::string MessageCoder::TIME_TO_END = GetNextKey("TIME_TO_END");
std::string MessageCoder::IS_FIRST_WORKER = GetNextKey("IS_FIRST_WORKER");

std::string MessageCoder::REASON = GetNextKey("REASON");
std::string MessageCoder::MAX_TIME = GetNextKey("MAX_TIME");
std::string MessageCoder::NUM_OF_ANALYSED = GetNextKey("NUM_OF_ANALYSED");
std::string MessageCoder::NUM_OF_BOARD = GetNextKey("NUM_OF_BOARD");
std::string MessageCoder::STATE = GetNextKey("STATE");
std::string MessageCoder::ROLE = GetNextKey("STATE");
std::string MessageCoder::JOB_ID = GetNextKey("JOB_ID");
std::string MessageCoder::NUM_OF_THREAD = GetNextKey("NUM_OF_THREAD");

std::string MessageCoder::PREVIOUS_MURDER = GetNextKey("PREVIOUS_MURDER");
std::string MessageCoder::WHITE_PATCH_END = GetNextKey("WHITE_PATCH_END");
std::string MessageCoder::BLACK_WHITE = GetNextKey("BLACK_WHITE");

std::string MessageCoder::NUM_OF_BOARD_TO_RETURN_FAST = GetNextKey("NUM_OF_BOARD_TO_RETURN_FAST");

std::string MessageCoder::ORIGIN_NUMBER_BLACK = GetNextKey("ORIGIN_NUMBER_BLACK");
std::string MessageCoder::ORIGIN_NUMBER_WHITE = GetNextKey("ORIGIN_NUMBER_WHITE");
std::string MessageCoder::ORIGIN_WHITE_X = GetNextKey("ORIGIN_WHITE_X_");
std::string MessageCoder::ORIGIN_WHITE_Y = GetNextKey("ORIGIN_WHITE_Y_");
std::string MessageCoder::ORIGIN_BLACK_X = GetNextKey("ORIGIN_BLACK_X_");
std::string MessageCoder::ORIGIN_BLACK_Y = GetNextKey("ORIGIN_BLACK_Y_");
std::string MessageCoder::ORIGIN_WHITE_PONS = GetNextKey("ORIGIN_WHITE_PONS_");
std::string MessageCoder::ORIGIN_BLACK_PONS = GetNextKey("ORIGIN_BLACK_PONS_");

std::string MessageCoder::NUMBER_BLACK = GetNextKey("NUMBER_BLACK");
std::string MessageCoder::NUMBER_WHITE = GetNextKey("NUMBER_WHITE");
std::string MessageCoder::WHITE_X = GetNextKey("WHITE_X_");
std::string MessageCoder::WHITE_Y = GetNextKey("WHITE_Y_");
std::string MessageCoder::BLACK_X = GetNextKey("BLACK_X_");
std::string MessageCoder::BLACK_Y = GetNextKey("BLACK_Y_");
std::string MessageCoder::WHITE_PONS = GetNextKey("WHITE_PONS_");
std::string MessageCoder::BLACK_PONS = GetNextKey("BLACK_PONS_");

unsigned int MessageCoder::nextKey = 0;
bool MessageCoder::debugMode = true;
unsigned long long MessageCoder::messageId = 0;
std::mutex MessageCoder::mutex_guard;
