#ifndef MESSAGECODER_H
#define MESSAGECODER_H

#include <cstring>
#include "../Traces/Traces.h"
#include "../Board/Board.h"
#include "../Board/PawnPos.h"
#include "../Peers/Peers.h"
#include "../ServerState/ServerState.h"
#include "../ProgramVariables/ProgramVariables.h"

class MessageCoder
{
    public:
        MessageCoder();
        static void KeyValuePairToChar(const std::string & key, const std::string & value, char *dest);

        enum ROLE_ENUM { CLIENT, WORKER };
        enum class NOT_OK_REASON { BUSY };

        static void KeyValuePairToChar(const std::string & key, const int value, char *dest);
        static void KeyValuePairToChar(const std::string & key, const unsigned int value, char *dest);
        static void KeyValuePairToChar(const std::string & key, const short value, char *dest);
        static void KeyValuePairToChar(const std::string & key, const long value, char *dest);
        static void KeyValuePairToChar(const std::string & key, const unsigned short value, char *dest);
        static void KeyValuePairToChar(const std::string & key, const unsigned long long value, char *dest);
        static void KeyValuePairToChar(const std::string & key, const bool value, char *dest);

        static void BoardToChar(const Board & board, char *dest, const unsigned short numberOfBoard);

        static void CreateCloseConnectionMessage(char *dest);
        static void CreateConnectedMessage(char *dest);
        static void CreateStartMessage(const unsigned long long respTime, const unsigned short numberOfBoard,  const std::string & id, const std::string & jobId, const Board & board, char *dest);
        static void CreateBestResultMessage(const Board & board, const std::string & id, const std::string & jobId, unsigned long long numOfAnalysed, char *dest);
        static void CreateGetServerStateMessage(const std::string & id, char *dest);
        static void CreateStateMessage(const Peers::STATE stat, const unsigned int numOfThread, const std::string & id, char *dest);
        static void CreateServerStateMessage(const ServerState & serverState, const std::string & id, char *dest);
        static void CreateRoleMessage(const ROLE_ENUM role, const std::string & id, char *dest);
        static void CreateTimeToSendResultToClientsMessage(char *dest);
        static void CreateTimeoutMessage(char *dest);
        static void CreateStartAnalyseWork(const unsigned long long respTime , const Board & board, const std::string & id, const std::string & jobId, const bool isFirstWorker, char *dest);
        static void CreateStartAnalyseWorkAndReturnNResultFast(const unsigned long long respTime, const unsigned int numOfResultToReturnFast, const Board & board, const std::string & id, const std::string & jobId, char *dest);
        static void CreateOkMessage(const std::string & id, char *dest);
        static void CreateNotOkMessage(const std::string & id, NOT_OK_REASON reason, char *dest);

        static void ClearChar(char *dest, const unsigned int num);        

        static void InsertHeader(char *dest);        
        static void InsertLenMessageHeader(char *dest);
        static void InsertLenMessageHeader(const unsigned int val, char *dest);
        static unsigned int HeaderToVal(char *dest);

        static void MessageToMap(const char *source, std::map<std::string, std::string> & dest);
        static void MapToBoard(const std::map<std::string, std::string> & dest, Board *board);

        static unsigned int MaxMessageSize() { return 4048; }
        static unsigned int MaxMessageConnectionCloseSize() { return 48; }
        static unsigned int MaxMessageConnectedSize() { return 48; }
        static unsigned int MaxMessageTimeToSendResultToClientsSize() { return 128; }
        static unsigned int MaxMessageTimeoutSize() { return 48; }
        static unsigned int BufferSize() { return 4; }

        static std::string CreateMessageId();

        //Keys
        static std::string ACTION;
        static std::string OK;
        static std::string NOT_OK;
        static std::string SERVER_STATE;
        static std::string MESSAGE_ID;
        static std::string MESSAGE_END;
        static std::string CLOSE_CNNECTION;
        static std::string CONNECTED;
        static std::string START_ANALYSE;
        static std::string START_ANALYSE_FAST;
        static std::string TIMEOUT;
        static std::string TIME_TO_SEND_RESULT_TO_CLIENTS;

        static std::string START_WORK;
        static std::string SET_STATE;        
        static std::string SET_ROLE;
        static std::string GET_SERVER_STATE;
        static std::string BEST_RESULT;

        static std::string IS_THINKING;
        static std::string MAX_IA_TIME;
        static std::string START_TIME;
        static std::string TIME_TO_END;
        static std::string IS_FIRST_WORKER;

        static std::string REASON;
        static std::string MAX_TIME;
        static std::string NUM_OF_ANALYSED;
        static std::string NUM_OF_BOARD;
        static std::string STATE;
        static std::string ROLE;
        static std::string JOB_ID;
        static std::string NUM_OF_THREAD;

        static std::string PREVIOUS_MURDER;
        static std::string WHITE_PATCH_END;
        static std::string BLACK_WHITE;

        static std::string NUM_OF_BOARD_TO_RETURN_FAST;

        static std::string ORIGIN_NUMBER_BLACK;
        static std::string ORIGIN_NUMBER_WHITE;
        static std::string ORIGIN_WHITE_X;
        static std::string ORIGIN_BLACK_X;
        static std::string ORIGIN_WHITE_Y;
        static std::string ORIGIN_BLACK_Y;
        static std::string ORIGIN_WHITE_PONS;
        static std::string ORIGIN_BLACK_PONS;

        static std::string NUMBER_BLACK;
        static std::string NUMBER_WHITE;
        static std::string WHITE_X;
        static std::string BLACK_X;
        static std::string WHITE_Y;
        static std::string BLACK_Y;
        static std::string WHITE_PONS;
        static std::string BLACK_PONS;

     private:
        static std::string GetNextKey(const std::string & debug_key);
        static unsigned int nextKey;
        static bool debugMode;
        static unsigned long long messageId;
        static std::mutex mutex_guard;
};

#endif // MESSAGECODER_H
