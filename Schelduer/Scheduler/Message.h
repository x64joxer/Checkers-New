#ifndef MESSAGE_H
#define MESSAGE_H


#include <boost/shared_ptr.hpp>
#include <cstring>
#include <SFML/Network.hpp>
#include "../Traces/Traces.h"
#include "../ProgramVariables/ProgramVariables.h"

class  TCPConnection;

typedef boost::shared_ptr<TCPConnection> TCPConnection_ptr;
typedef boost::shared_ptr<char> DataShared_ptr;

class Message
{
    public:         
        Message();
        Message(const Message & data);

        void CopyData(TCPConnection_ptr connection, const char *wskM);
        void CopyWsk(TCPConnection_ptr connection, char *wskM);
        void Clear() { connectionWsk.reset(); wskMessage.reset(); }
        char *GetWskMessage() const { return wskMessage.get(); }
        TCPConnection_ptr GetTCPConnection_ptr() const { return connectionWsk; }

        Message & operator=(const Message  & data);
        ~Message();

    private:
        TCPConnection_ptr connectionWsk;
        DataShared_ptr wskMessage;
};

#endif // MESSAGE_H
