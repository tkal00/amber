#ifndef AMBER_CORE_CONFIG
#define AMBER_CORE_CONFIG
#include <string_view>

namespace amber::config
{
    auto webRootPath() -> std::string_view;
}

#endif
