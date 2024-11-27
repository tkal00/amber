#ifndef AMBER_HEADER_MESSAGE
#define AMBER_HEADER_MESSAGE
#include "messageTypes.h"
#include "util/stringMap.h"

namespace amber
{

namespace http
{
    enum class Method
    {
        invalid = 0,
        put,
        post,
        get,
        del,
        patch,
        head,
        options
    };

    auto fileExtToMimeType(std::string_view ext) -> std::string_view;

    class Message
    {
    protected:
        Message() {}

    public:
        void setHeader(std::string_view name, std::string_view value);
        void setHeader(std::string_view name, std::integral auto value) { auto tmp = std::to_string(value); setHeader(name, tmp); }
        auto getHeader(std::string_view name) -> std::string_view;
        void setBody(std::string_view body);
        auto getBody() -> std::string_view;

    protected:
        util::StringMapUnordered<std::string> m_headers;
        std::string m_body;
    };

    class Request : public Message
    {
    public:
        explicit Request(std::string_view data);
        auto getPath()      -> std::string_view;
        auto getMethod()    -> Method;

    private:
        Method m_method;
        std::string m_path;
    };

    class Response : public Message
    {
    public:
        Response();
        Response(StatusCode status);

        void setStatus(StatusCode status) { m_status = status; }
        auto toString(bool includeBody = true) -> std::string;

    private:
        StatusCode m_status;
    };
}

}
#endif
