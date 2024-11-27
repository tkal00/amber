#include "session.h"

auto amber::http::Session::getConnection() -> const Connection&
{
        return m_conn;
}
