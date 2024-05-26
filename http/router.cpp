#include "router.h"

// Route
amber::http::Route::Route(std::string path) 
    : m_path(std::move(path)) 
{
}

amber::http::Route::~Route()
{
}

auto amber::http::Route::get(CallbackFn fn) -> Route&
{
    m_callbackGet = fn;
    return *this;
}

auto amber::http::Route::invokeGet(Request& req, Response& res) -> bool
{
    return m_callbackGet(req, res);
}

auto amber::http::Route::invokePost(Request& req, Response& res) -> bool
{
    return m_callbackPost(req, res);
}

// Router
amber::http::Router::Router()
{
}

amber::http::Router::~Router()
{
}

auto amber::http::Router::addRoute(std::string_view path) -> Route&
{
    std::string strPath(path);
    auto found = m_routes.find(strPath);
    if (found == m_routes.end())
        found = m_routes.try_emplace(strPath, strPath).first;
    return found->second;
}

auto amber::http::Router::getRoute(std::string_view path) -> Route*
{
    if (auto found = m_routes.find(std::string(path)); found != m_routes.end())
        return &found->second;
    return nullptr;
}

