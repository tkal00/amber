#ifndef AMBER_HTTP_ROUTER_PUBLIC
#define AMBER_HTTP_ROUTER_PUBLIC
#include "http/router.h"

namespace amber::http
{
    class StaticRouter : public Router
    {
    public:
        StaticRouter(std::string_view path);

    private:
        std::string_view m_staticPath;
    };
}

#endif
