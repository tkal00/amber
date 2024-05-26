#ifndef AMBER_HTTP_SERVER
#define AMBER_HTTP_SERVER
#include "router.h"
#include "core/server.h"
#include <vector>
#include <memory>

namespace amber::http
{
    class HttpServer : public Server
    {
    public:
        HttpServer();
        ~HttpServer();

        bool start(int port);
        void stop();

        void processRequest();
        Router& addRouter();

    private:
        int m_listener;
        bool m_isActive;
        std::vector<std::shared_ptr<Router>> m_routers;
    };
}

#endif
