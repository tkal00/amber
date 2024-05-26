#ifndef AMBER_HTTP_ROUTER
#define AMBER_HTTP_ROUTER
#include "message.h"
#include <memory>
#include <string>

namespace amber::http
{
    class Route
    {
    public:
        using self = Route&;
        using CallbackFn = bool(*)(Request&, Response&);
        Route(std::string);
        Route(const Route&) = delete;
        Route& operator=(const Route&) = delete;
        ~Route();

        auto get(CallbackFn fn) -> Route&;
        auto post(CallbackFn fn) -> Route&;

        auto invokeGet(Request&, Response&) -> bool;
        auto invokePost(Request&, Response&) -> bool;

    private:
        std::string m_path;
        CallbackFn m_callbackGet;
        CallbackFn m_callbackPost;
    };

    class Router
    {
    public:
        Router();
        Router(const Router&) = delete;
        Router& operator=(const Router&) = delete;
        ~Router();

        static Router& get() { static Router instance; return instance; }
        auto addRoute(std::string_view path) -> Route&;
        auto getRoute(std::string_view path) -> Route*;

    private:
        std::unordered_map<std::string, Route> m_routes;
    };
}

#endif
