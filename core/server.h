#ifndef AMBER_SERVER
#define AMBER_SERVER
namespace amber
{

    class Server
    {
    public:

    protected:
        Server();
        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;
        virtual ~Server();
    };

}

#endif
