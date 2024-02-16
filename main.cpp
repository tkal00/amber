#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>

constexpr unsigned c_bufferSize = 1024*1024;
constexpr int c_port = 3001;

std::string createSimpleResponse(std::string_view msg, int status = 200)
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
    std::cout << "ERROR: " << msg << '\n';
    exit(1);
}

int main(int argc, const char** argv)
{
    sockaddr_in serverAddress, clientAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(c_port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        error("listening socket failed to create");
    if (bind(sockfd, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        error("couldn't bind listening socket");
    listen(sockfd, 1024);

    int clilen = sizeof(clientAddress);
    auto res = createSimpleResponse("<h1>hello world</h1>");
    while (true)
    {
        int acceptfd = accept(sockfd, nullptr, nullptr);
        if (acceptfd < 0)
        {
            std::cout << "mangled connection\n";
            continue;
        }
        int n = send(acceptfd, res.data(), res.length(), 0);
        if (n < 0)
            std::cout << "failed to send response\n";
        shutdown(acceptfd, 2);
    }
    return 0;
}
