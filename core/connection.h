#ifndef AMBER_CONNECTION
#define AMBER_CONNECTION
#include <string>

namespace amber
{
        class Connection
        {
        public:
                Connection(int fd);
                auto operator=(Connection&& other) -> Connection&;
                Connection(Connection&& other);
                virtual ~Connection();

                virtual auto read() -> std::string;
                virtual auto send(std::string_view data) -> bool;
                operator bool() const { return m_fd != -1; }
        private:
                int m_fd;
        };
}

#endif
