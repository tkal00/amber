#ifndef AMBER_UTIL_STRINGSET
#define AMBER_UTIL_STRINGSET
#include "stringCompare.h"
#include <set>

namespace amber::util
{
    using StringSet = std::set<std::string, Compare>;
}

#endif
