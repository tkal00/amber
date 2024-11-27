#include "staticrouter.h"

amber::http::StaticRouter::StaticRouter(std::string_view path)
    : m_staticPath(path)
{
}
