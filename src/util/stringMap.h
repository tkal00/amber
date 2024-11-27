#ifndef AMBER_HEADER_STRINGMAP
#define AMBER_HEADER_STRINGMAP
#include "stringCompare.h"
#include <map>
#include <unordered_map>

namespace amber::util
{
    struct KeyCompare
    {
        using is_transparent = void;
        std::size_t operator()(const std::string_view& a)                       const { return std::hash<std::string_view>()(a); };
        std::size_t operator()(const std::string& a)                            const { return std::hash<std::string>()(a); };
        bool operator()(const std::string_view& a, const std::string_view& b)   const { return KeyCompare()(a) < KeyCompare()(b); };
        bool operator()(const std::string& a, const std::string& b)             const { return KeyCompare()(a) < KeyCompare()(b); };
    };
    struct KeyEqual
    {
        using is_transparent = void;
        std::size_t operator()(const std::string_view& a)                       const { return std::hash<std::string_view>()(a); };
        std::size_t operator()(const std::string& a)                            const { return std::hash<std::string>()(a); };
        bool operator()(const std::string_view& a, const std::string_view& b)   const { return KeyCompare()(a) == KeyCompare()(b); };
        bool operator()(const std::string& a, const std::string& b)             const { return KeyCompare()(a) == KeyCompare()(b); };
    };
    template<typename T>
    using StringMap = std::map<std::string, T, KeyCompare>;
    template<typename T>
    using StringMapUnordered = std::unordered_map<std::string, T, KeyEqual, KeyEqual>;
}

#endif
