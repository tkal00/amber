#ifndef AMBER_HEADER_STRING_MAP
#define AMBER_HEADER_STRING_MAP
#include <map>
#include <unordered_map>
#include <string>
#include <string_view>

namespace amber
{

namespace utils
{

struct Compare
{
    using is_transparent = void;
    std::size_t operator()(const std::string_view& a)                       const { return std::hash<std::string_view>()(a); };
    std::size_t operator()(const std::string& a)                            const { return std::hash<std::string>()(a); };
    bool operator()(const std::string_view& a, const std::string_view& b)   const { return Compare()(a) < Compare()(b); };
    bool operator()(const std::string& a, const std::string& b)             const { return Compare()(a) < Compare()(b); };
};

using StringMap             = std::map<             std::string,    std::string,    Compare>;
using StringMapUnordered    = std::unordered_map<   std::string,    std::string,    Compare>;

}

}

#endif
