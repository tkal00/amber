#include "server.h"
#include "log/log.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

amber::Server::Server()
        : m_listener(-1)
        , m_port(-1)
        , m_isActive(false)
{
}

amber::Server::~Server()
{
        stop();
}

auto amber::Server::listen(int port, int queueSize /* = 1024 */) -> bool
{
    auto logErrorAndQuit = [&](std::string_view msg) -> bool
    {
        LOG_ERROR(msg << ", " << LOG_ERRNO());
        stop();
        return false;
    };

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    m_listener = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listener == -1)
        return logErrorAndQuit("couldn't create listener");

    static struct linger ling = []() -> linger
    {
        linger ling;
        ling.l_onoff = 1;
        ling.l_linger = 0;
        return ling;
    }();
    if (setsockopt(m_listener, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) < 0)
        return logErrorAndQuit("setsockopt failed");
    int reuseaddr = 1;
    if (setsockopt(m_listener, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) < 0)
        return logErrorAndQuit("setsockopt failed");
    int keepalive = 1;
    if (setsockopt(m_listener, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(int)) < 0)
        return logErrorAndQuit("setsockopt failed");
    if (bind(m_listener, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        return logErrorAndQuit("couldn't bind listener");
    ::listen(m_listener, queueSize);

    m_port = port;
    LOG_TRACE("new server listening on port " << m_port);
    m_isActive = true;
    return true;
}

void amber::Server::stop()
{
    if (m_listener != -1)
    {
        ::shutdown(m_listener, SHUT_RDWR);
        ::close(m_listener);
    }
    LOG_TRACE("server stopped listening on port " << m_port);
    m_port = -1;
    m_isActive = false;
}

auto amber::Server::accept() -> Connection
{
        int fd = (::accept(m_listener, nullptr, nullptr));
        LOG_TRACE("accepting new connection: " << fd);
        if (fd == -1)
                LOG_INFO(LOG_ERRNO());
        return {fd};
}
