#ifndef AMBER_CORE_CONFIG
#define AMBER_CORE_CONFIG
#include <string_view>
#include "log/log.h"

namespace amber::config
{
        auto loadEnv(std::string_view name) -> const char*;
#define DEFINE_VAR(varName, fnName) \
        inline auto fnName() -> std::string_view \
        { \
                static const std::string_view val(amber::config::loadEnv(varName)); \
                return val; \
        }
        DEFINE_VAR("AMBER_WEBROOT", webRootPath);
        DEFINE_VAR("AMBER_PUBLICROOT", publicRootPath);
}

#endif
