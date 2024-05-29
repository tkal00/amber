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

        void handleRequest();

        template<typename T, typename... Args> requires std::is_same_v<T, Router> || std::is_base_of_v<Router, T>
        auto pushRouter(Args&&... args) -> std::shared_ptr<T> 
        {
            auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
            m_routers.push_back(ptr);
            return ptr;
        }
        auto pushRouter() -> std::shared_ptr<Router> { return pushRouter<Router>(); }

    private:
        int m_listener;
        bool m_isActive;
        std::vector<std::shared_ptr<Router>> m_routers;
    };
}

#endif
