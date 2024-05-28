#ifndef AMBER_HTML_PARSER
#define AMBER_HTML_PARSER
#include "htmltypes.h"
#include <stack>

namespace amber::html
{
    class HtmlParser
    {
    public:
        auto parse(std::istream& input) -> HtmlDocument;
        auto operator()(std::istream& input) -> HtmlDocument;
    private:
        std::stack<HtmlElement> m_parentStack;
    };
}

#endif
