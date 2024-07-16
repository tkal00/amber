#include "router.h"
#include "log/log.h"
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

void amber::http::Router::addPreHandler(CallbackFn fn)
{
        m_preHandlers.push_back(fn);
}

void amber::http::Router::addPostHandler(CallbackFn fn)
{
        m_postHandlers.push_back(fn);
}

auto amber::http::Router::handleRequest(Request& request, Response& response) -> bool
{
        bool ret = false;
        // TODO: rethink ret val of routes and handlers, do they consume or continue on true?
#define handleMethod(m) \
        if (auto* fn = route->m) \
                ret = fn(request, response);

        for (auto preFn : m_preHandlers)
                if (!preFn(request, response))
                        return true;
        auto* route = getRoute(request.getPath());
        if (route)
                switch (request.getMethod())
                {
                case Method::get:   handleMethod(get); break;
                case Method::post:  handleMethod(post); break;
                case Method::put:   handleMethod(put); break;
                case Method::del:   handleMethod(del); break;
        
                case Method::invalid: 
                        LOG_ERROR("invalid request HTTP method");
                        break;
        
                default: LOG_ERROR("reached default case of method switch");
                }
#undef handleMethod
        for (auto fn : m_postHandlers)
                fn(request, response);
        return ret;
}

