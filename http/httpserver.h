#ifndef AMBER_HTTP_SERVER
#define AMBER_HTTP_SERVER
#include "router.h"
#include "core/server.h"
#include <memory>
#include <set>
#include <vector>

namespace amber::http
{
    class HttpServer : public Server
    {
    public:
        HttpServer();
        ~HttpServer();

        void pollConnections();
        void handleRequest(Connection&& conn);

        template<typename T, typename... Args> requires std::is_same_v<T, Router> || std::is_base_of_v<Router, T>
        auto pushRouter(Args&&... args) -> std::shared_ptr<T> 
        {
            auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
            m_routers.push_back(ptr);
            return ptr;
        }
        auto pushRouter() -> std::shared_ptr<Router> { return pushRouter<Router>(); }

    private:
        void handleConnection_threaded(Connection&& conn);
        std::vector<std::shared_ptr<Router>> m_routers;
        std::vector<Connection> m_connections; // TODO: replace with sessions
    };
}

#endif
