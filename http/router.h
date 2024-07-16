#ifndef AMBER_HTTP_ROUTER
#define AMBER_HTTP_ROUTER
#include "message.h"
#include <vector>

namespace amber::http
{
        class Router
        {
        public:
                using CallbackFn = bool(*)(Request&, Response&);
                Router();
                Router(const Router&) = delete;
                Router& operator=(const Router&) = delete;
                ~Router();

                auto get(std::string_view path, CallbackFn fn)  -> decltype(*this);
                auto post(std::string_view path, CallbackFn fn) -> decltype(*this);
                auto put(std::string_view path, CallbackFn fn)  -> decltype(*this);
                auto del(std::string_view path, CallbackFn fn)  -> decltype(*this);

                // basically middleware ala nodejs express per route
                void addPreHandler(CallbackFn fn);
                void addPostHandler(CallbackFn fn);

                // returns true if request has been consumed (?)
                virtual auto handleRequest(Request&, Response&) -> bool;

        private:
                struct Route
                {
                        Route(int unused) {};
                        CallbackFn get      = nullptr;
                        CallbackFn post     = nullptr;
                        CallbackFn put      = nullptr;
                        CallbackFn del      = nullptr;
                };

                auto getOrAddRoute(std::string_view path) -> Route&;
                auto getRoute(std::string_view path) -> Route*;

                util::StringMapUnordered<Route> m_routes;
                std::vector<CallbackFn> m_preHandlers;
                std::vector<CallbackFn> m_postHandlers;
        };
}

#endif
