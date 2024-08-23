#include "connection.h"
#include "log/log.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std::literals;

amber::Connection::Connection(int fd)
        : m_fd(fd)
{
}

amber::Connection::Connection(Connection&& other)
        : m_fd(other.m_fd)
{
        other.m_fd = -1;
}

auto amber::Connection::operator=(Connection&& other) -> Connection&
{
        this->m_fd = other.m_fd;
        other.m_fd = -1;
        return *this;
}

amber::Connection::~Connection()
{
        if (m_fd != -1)
        {
                LOG_TRACE("closing connection: " << m_fd);
                ::shutdown(m_fd, SHUT_RDWR);
                ::close(m_fd);
        }
}

auto amber::Connection::read() -> std::string
{
        int recvBytes = 0;
        std::string buf;
        buf.reserve(4096);
        constexpr int stack_buf_size = 4096; 
        char buffer[stack_buf_size];
        char peekBuf;
        recvBytes = recv(m_fd, &peekBuf, 1, MSG_PEEK);
        do {
                recvBytes = recv(m_fd, buffer, stack_buf_size, MSG_DONTWAIT);
                if (recvBytes > 0)
                        buf.append(buffer, recvBytes);
        } while (recvBytes > 0);
        LOG_TRACE(buf);
        return buf;
}

auto amber::Connection::send(std::string_view data) -> bool
{
        long sent = ::send(m_fd, data.data(), data.length(), 0);
        if (sent == -1)
                LOG_ERROR("couldnt send data: " << LOG_ERRNO());
        else if (sent != data.length())
                LOG_WARN("incomplete send: " << sent << " instead of " << data.length());
        return sent == data.length();
}
