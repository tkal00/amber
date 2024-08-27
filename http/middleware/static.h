#ifndef AMBER_HTTP_MIDDLEWARE_STATIC
#define AMBER_HTTP_MIDDLEWARE_STATIC
#include "http/message.h"

namespace amber::http::middleware
{
        auto serveStatic(Request& req, Response& res) -> bool;
        auto useUtf8(Request& req, Response& res) -> bool;
}

#endif
