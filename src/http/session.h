#ifndef AMBER_HTTP_SESSION
#define AMBER_HTTP_SESSION
#include "core/connection.h"

namespace amber::http
{
        class Session
        {
        public:
                Session();
                ~Session();

                auto getConnection() -> const Connection&;

        private:
                Connection m_conn;
        };
}

#endif
