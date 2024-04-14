#include "http/message.h"
#include <iostream>
#include <netinet/in.h>
#include <map>
#include <sys/socket.h>
#include <signal.h>
#include <sstream>
#include <string>
#include <string.h>

constexpr unsigned c_bufferSize = 1024*1024;
constexpr int c_port = 3001;

inline std::string createSimpleResponse(std::string_view msg, int status = 200)
{
    std::map<int, std::string_view> statusMsg = 
    {
        { 200, "OK" },
        { 404, "Not found" },
    };

    std::stringstream ss;
    ss  << "HTTP/1.1 " << status << ' ' << statusMsg[status] << '\n'
        << '\n'
        << msg;
    return ss.str();
}

void error(std::string_view msg)
{
    auto err = errno;
    std::cout << "ERROR: " << msg << ", " << err << ": " << strerror(err) << '\n';
    exit(1);
}

struct serverSockRAII
{
    serverSockRAII()
        : fd(-1)
    {
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(c_port);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1)
            error("listening socket failed to create");
        static struct linger ling;
        ling.l_onoff = 1;
        ling.l_linger = 0;
        if (setsockopt(fd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) < 0)
            error("setsockopt: ");
        int reuseaddr = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) < 0)
            error("setsockopt: ");
        if (bind(fd, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
            error("couldn't bind listening socket");
        listen(fd, 1024);
    }

    ~serverSockRAII()
    {
        if (fd != -1)
        {
            shutdown(fd, SHUT_RDWR);
            
            close(fd);
        }
    }

    int fd;
};

bool g_running = true;

void handleInterrupt(int s)
{
    g_running = false;
}

int main(int argc, const char** argv)
{
    struct sigaction sigInterruptHandler;
    sigInterruptHandler.sa_handler = &handleInterrupt;
    sigemptyset(&sigInterruptHandler.sa_mask);
    sigaction(SIGINT, &sigInterruptHandler, NULL);

    serverSockRAII server;
    auto res = createSimpleResponse("<h1>hello world</h1>");

    while (g_running)
    {
        int acceptfd = accept(server.fd, nullptr, nullptr);
        if (acceptfd < 0)
        {
            std::cout << "mangled connection\n";
            continue;
        }
        int recvBytes = 0;
        std::string request;
        request.reserve(1024);
        constexpr int c_readBufferSize = 512;
        char buf[c_readBufferSize];
        char peekBuf;
        recvBytes = recv(acceptfd, &peekBuf, 1, MSG_PEEK);
        do {
            recvBytes = recv(acceptfd, buf, c_readBufferSize, MSG_DONTWAIT);
            if (recvBytes > 0)
                request.append(buf, recvBytes);
        } while (recvBytes > 0);
        if (!request.empty())
            std::cout << request << '\n';

        std::cout << "Request Obj: \n";
        amber::http::Request obj(request);
        std::cout << "with path: " << obj.getPath() << '\n';

        int n = send(acceptfd, res.data(), res.length(), 0);
        if (n < 0)
            std::cout << "failed to send response\n";
        shutdown(acceptfd, SHUT_RDWR);
        close(acceptfd);
    }
    return 0;
}
