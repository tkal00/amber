#include "config.h"
#include <cstdlib>
#include <string>

auto amber::config::webRootPath() -> std::string_view
{
    static const std::string buf([]() -> const char* { return std::getenv("AMBER_ROOTDIR"); }());
    return buf;
}
