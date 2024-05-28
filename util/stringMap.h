#ifndef AMBER_HEADER_STRINGMAP
#define AMBER_HEADER_STRINGMAP
#include "stringCompare.h"
#include <map>
#include <unordered_map>

namespace amber::util
{
    template<typename T>
    using StringMap = std::map<std::string, T, Compare>;
    template<typename T>
    using StringMapUnordered = std::unordered_map<std::string, T, Compare>;
}

#endif
