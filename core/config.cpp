#include "config.h"
#include <cstdlib>
#include <iostream>
#include <string>

namespace
{
    auto loadEnv(std::string_view name) -> const char*
    {
        const char* cstr = std::getenv(name.data());
        // i need some basic logger soon
        if (!cstr)
            std::cout << "env variable '" << name << "' empty or not set\n";
        return cstr;
    }
}

auto amber::config::webRootPath() -> std::string_view
{
    static const std::string_view buf(loadEnv("AMBER_WEBROOT"));
    return buf;
}

auto amber::config::publicRootPath() -> std::string_view
{
    static const std::string_view buf(loadEnv("AMBER_PUBLICROOT"));
    return buf;
}
