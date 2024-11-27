#include "htmltypes.h"
#include "util/strings.h"
#include <algorithm>
#include <sstream>

namespace
{
    auto isEmptyElement(std::string_view tag) -> bool
    {
        // https://developer.mozilla.org/en-US/docs/Glossary/Void_element
        static const amber::util::StringSet specialTags =
        {
            "area",
            "base",
            "br",
            "col",
            "embed",
            "hr",
            "img",
            "input",
            "link",
            "meta",
            "param",
            "source",
            "track",
            "wbr"
        };
        return specialTags.find(tag) != specialTags.end();
    }
}

amber::html::HtmlElement::HtmlElement(std::string_view tagName)
{
    auto found = std::ranges::lower_bound(s_tagNameCache, tagName);
    if (*found != tagName)
        s_tagNameCache.emplace_hint(found, tagName);
    tagName = *found;
}

void amber::html::HtmlElement::toString(std::ostream& os)
{
    os << '<' << tagName;
    for (const auto& [name, value] : attributes)
        os << ' ' << name << '=' << '"' << value << '"';
    os << '>';
    if (isEmptyElement(tagName))
        return;
    os << content;
    for (auto& child : children)
        child.toString(os);
    os << '<' << '/' << tagName << '>';
}

auto amber::html::HtmlElement::toString() -> std::string
{
    std::stringstream ss;
    this->toString(ss);
    return ss.str();
}
amber::util::StringSet amber::html::HtmlElement::s_tagNameCache;

amber::html::HtmlDocument::HtmlDocument()
    : root("html")
{
}
