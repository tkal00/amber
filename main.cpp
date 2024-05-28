#include "core/config.h"
#include "http/httpserver.h"
#include "http/messageTypes.h"
#include <iostream>
#include <netinet/in.h>
#include <fstream>
#include <map>
#include <sys/socket.h>
#include <signal.h>
#include <sstream>
#include <string>
#include <string.h>
#include <stdlib.h>

constexpr unsigned c_bufferSize = 1024*1024;
constexpr int c_port = 3001;

std::string readFile(std::string path)
{
    path.insert(0, "/").insert(0, amber::config::webRootPath());
    std::stringstream outputBuffer;
    std::ifstream file;
    file.open(path, std::iostream::binary);
    if (!file.is_open() || !file.good())
        std::cerr << "couldnt open path: " << path << '\n';
    outputBuffer << file.rdbuf();
    return outputBuffer.str();
}

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

    std::string g_rootPath = std::getenv("AMBER_ROOTDIR");
    amber::http::HttpServer server;
    auto& defaultRouter = server.addRouter();
    defaultRouter.addRoute("/")
        .get([](auto& req, auto& res) -> bool
            {
                res.setBody(readFile("index.html"));
                res.setStatus(amber::http::ok_200);
                return true;
            });
    defaultRouter.addRoute("/css/styles.css")
        .get([](auto& req, auto& res) -> bool
            {
                res.setBody(readFile("css/styles.css"));
                res.setStatus(amber::http::ok_200);
                return true;
            });
    defaultRouter.addRoute("/js/welcome.js")
        .get([](auto& req, auto& res) -> bool
            {
                res.setBody(readFile("js/welcome.js"));
                res.setStatus(amber::http::ok_200);
                return true;
            });
    server.start(c_port);
    while (g_running)
    {
        server.processRequest();
    }
    server.stop();
    return 0;
}
