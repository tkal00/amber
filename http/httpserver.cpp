#include "httpserver.h"
#include "log/log.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

amber::http::HttpServer::HttpServer()
{
}

amber::http::HttpServer::~HttpServer()
{
}

void amber::http::HttpServer::handleConnection_threaded(amber::Connection&& conn)
{
        while (conn) {
                auto raw = conn.read();
                if (raw.empty())
                        return;
                amber::http::Request request(raw);
                amber::http::Response response;
                for (std::shared_ptr<Router> router : m_routers) {
                        if (router->handleRequest(request, response))
                                break;
                }
                response.setHeader("Content-Length", response.getBody().length());
                // MAYBE: enforce the http/1.0 keep-alive design of the server being unable to
                // push changes to the client without a prior request (HTTPConnection/Session)
                int n = conn.send(response.toString());
                if (!n)
                        LOG_ERROR("failed to send response, " << LOG_ERRNO());
        }
}

// using a pipeline design instead of rigid function flow might be interesting
void amber::http::HttpServer::handleRequest(Connection&& conn)
{
        std::thread(&HttpServer::handleConnection_threaded, this, std::forward<Connection>(conn)).detach();
}
