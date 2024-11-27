#ifndef AMBER_UTIL_STRINGCOMPARE
#define AMBER_UTIL_STRINGCOMPARE
#include <string>
#include <string_view>

namespace amber::util
{
    struct Compare
    {
        using is_transparent = void;
        std::size_t operator()(const std::string_view& a)                       const { return std::hash<std::string_view>()(a); };
        std::size_t operator()(const std::string& a)                            const { return std::hash<std::string>()(a); };
        bool operator()(const std::string_view& a, const std::string_view& b)   const { return Compare()(a) < Compare()(b); };
        bool operator()(const std::string& a, const std::string& b)             const { return Compare()(a) < Compare()(b); };
    };
}

#endif
