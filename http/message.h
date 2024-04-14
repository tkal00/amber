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
        del 
    };

    class Message
    {
    protected:
        Message() : m_isValid(false) {}

    public:
        void            setHeader(std::string_view name, std::string_view value)    ;
        void            setBody(std::string_view body)                              ;
        inline bool     isValid()                                                   { return m_isValid; }

    protected:
        bool m_isValid;

        utils::StringMap m_headers;
        std::string m_body;
    };

    class Request : public Message
    {
    public:
        explicit Request(std::string_view data);

        std::string_view getPath()                                                  { return m_path; }

    private:
        Method m_method;
        std::string m_path;
    };

    class Response : public Message
    {
    public:
        explicit Response(StatusCode status);

    private:
        StatusCode m_status;
    };
}

}
#endif
