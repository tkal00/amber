#include "httpserver.h"
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

amber::http::HttpServer::HttpServer()
    : m_listener(-1)
    , m_isActive(false)
{
}

amber::http::HttpServer::~HttpServer()
{
}

bool amber::http::HttpServer::start(int port)
{
    auto logErrorAndQuit = [&](std::string_view msg) -> bool
    {
        auto err = errno;
        std::cout << "ERROR: " << msg << ", " << err << ": " << strerror(err) << '\n';
        stop();
        return false;
    };

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    m_listener = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listener == -1)
        return logErrorAndQuit("listening socket failed to create");
    static struct linger ling = []() -> linger
    {
        linger ling;
        ling.l_onoff = 1;
        ling.l_linger = 0;
        return ling;
    }();
    if (setsockopt(m_listener, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) < 0)
        return logErrorAndQuit("setsockopt: ");
    int reuseaddr = 1;
    if (setsockopt(m_listener, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) < 0)
        return logErrorAndQuit("setsockopt: ");
    if (bind(m_listener, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        return logErrorAndQuit("couldn't bind listening socket");
    listen(m_listener, 1024);

    m_isActive = true;
    return true;
}

void amber::http::HttpServer::stop()
{
    if (m_listener != -1)
    {
        ::shutdown(m_listener, SHUT_RDWR);
        ::close(m_listener);
    }
    m_isActive = false;
}

// using a pipeline design instead of rigid function flow might be interesting
void amber::http::HttpServer::handleRequest()
{
    int fd = accept(m_listener, nullptr, nullptr);
    if (fd < 0)
    {
        std::cout << "mangled connection\n";
        return;
    }
    int recvBytes = 0;
    std::string rawRequest;
    rawRequest.reserve(1024);
    constexpr int c_readBufferSize = 512;
    char buf[c_readBufferSize];
    char peekBuf;
    recvBytes = recv(fd, &peekBuf, 1, MSG_PEEK);
    do {
        recvBytes = recv(fd, buf, c_readBufferSize, MSG_DONTWAIT);
        if (recvBytes > 0)
            rawRequest.append(buf, recvBytes);
    } while (recvBytes > 0);
    if (!rawRequest.empty())
        std::cout << rawRequest << '\n';

    std::cout << "Request Obj: \n";
    amber::http::Request request(rawRequest);
    amber::http::Response response;
    for (std::shared_ptr<Router> router : m_routers)
    {
        if (router->handleRequest(request, response))
            break;
    }
    auto resString = response.toString();
    std::cout << "Response Obj: \n" << response.toString(false) << '\n';
    int n = send(fd, resString.data(), resString.length(), 0);
    if (n < 0)
        std::cout << "failed to send response\n";
    shutdown(fd, SHUT_RDWR);
    close(fd);
}
