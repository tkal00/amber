#include "router.h"
#include <iostream>

amber::http::Router::Router()
{
}

amber::http::Router::~Router()
{
}

auto amber::http::Router::get(std::string_view path, CallbackFn fn) -> Router&
{
    getOrAddRoute(path).get = fn;
    return *this;
}

auto amber::http::Router::post(std::string_view path, CallbackFn fn) -> Router&
{
    getOrAddRoute(path).post = fn;
    return *this;
}

auto amber::http::Router::put(std::string_view path, CallbackFn fn) -> Router&
{
    getOrAddRoute(path).put = fn;
    return *this;
}

auto amber::http::Router::del(std::string_view path, CallbackFn fn) -> Router&
{
    getOrAddRoute(path).del = fn;
    return *this;
}

auto amber::http::Router::getOrAddRoute(std::string_view path) -> Route&
{
    auto found = m_routes.find(path);
    if (found == m_routes.end())
        found = m_routes.emplace(path, 0).first;
    return found->second;
}

auto amber::http::Router::getRoute(std::string_view path) -> Route*
{
    if (auto found = m_routes.find(path); found != m_routes.end())
        return &found->second;
    return nullptr;
}

auto amber::http::Router::handleRequest(Request& request, Response& response) -> bool
{
    auto* route = getRoute(request.getPath());
    if (!route)
        return false;

#define handleMethod(m)                 \
    if (auto* fn = route->m)            \
        return fn(request, response);   \
    else                                \
        return false

    switch (request.getMethod())
    {
        case Method::get:   handleMethod(get);
        case Method::post:  handleMethod(post);
        case Method::put:   handleMethod(put);
        case Method::del:   handleMethod(del);

        case Method::invalid: 
            std::cout << "invalid method from HTTP request\n"; 
            return false;
        default: std::cout << "to be supported\n"; return false;
    }
#undef handleMethod
    std::cout << __FILE__ << ':' << __LINE__ << ": something went horribly wrong here\n";
    return false;
}

