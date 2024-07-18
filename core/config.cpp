#include "config.h"
#include <cstdlib>

auto amber::config::loadEnv(std::string_view name) -> const char*
{
        static constexpr char empty = '\0';
        if (const char* cstr = std::getenv(name.data()))
                return cstr;
        LOG_WARN("env var '" << name << "' empty or not set");
        return &empty;
}
