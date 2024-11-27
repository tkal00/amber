#ifndef AMBER_SERVER
#define AMBER_SERVER
#include "connection.h"

namespace amber
{

    class Server
    {
    public:
        Server();
        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;
        virtual ~Server();

        bool listen(int port, int queueSize = 1024);
        void stop();

        auto accept() -> Connection;

    protected:
        int m_listener;
        int m_port;
        bool m_isActive;

    };

}

#endif
