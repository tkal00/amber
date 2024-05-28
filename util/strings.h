#ifndef AMBER_UTIL_STRINGS
#define AMBER_UTIL_STRINGS
#include <string_view>

namespace amber::util::string
{
    auto isNumeric(std::string_view) -> bool;
}

#endif
