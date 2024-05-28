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

    class Message
    {
    protected:
        Message() {}

    public:
        void setHeader(std::string_view name, std::string_view value);
        void setBody(std::string_view body);

    protected:
        util::StringMap<std::string> m_headers;
        std::string m_body;
    };

    class Request : public Message
    {
    public:
        explicit Request(std::string_view data);
        auto getPath() -> std::string_view { return m_path; }

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
        auto toString() -> std::string;

    private:
        StatusCode m_status;
    };
}

}
#endif
