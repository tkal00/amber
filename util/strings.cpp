#include "strings.h"
#include <algorithm>
#include <cctype>

auto amber::util::string::isNumeric(std::string_view sv) -> bool
{
    auto begin = sv.begin();
    if (sv.empty())
        return false;
    else if (*begin == '-')
        ++begin;
    return std::find_if(begin, sv.end(), [](unsigned char c){ return !std::isdigit(c); }) == sv.end();
}
