#ifndef AMBER_HTML_TYPES
#define AMBER_HTML_TYPES
#include "util/stringMap.h"
#include "util/stringSet.h"
#include <vector>

namespace amber::html
{
    struct HtmlElement 
    {
        explicit HtmlElement(std::string_view tagName);
        virtual void toString(std::ostream& os);
        virtual auto toString() -> std::string;
        std::string_view                        tagName;
        std::string                             content;
        util::StringMapUnordered<std::string>   attributes;
        std::vector<HtmlElement>                children;
    protected:
        static util::StringSet s_tagNameCache;
    }; 

    struct HtmlDocument
    {
        HtmlDocument();
        HtmlElement root;
    };

    auto parseHtml(std::istream input) -> HtmlDocument;
}

#endif
