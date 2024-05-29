#include "message.h"
#include "http/messageTypes.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std::literals;

auto amber::http::fileExtToMimeType(std::string_view ext) -> std::string_view
{
    static const util::StringMapUnordered<std::string_view> map =
    {
        { ".ico",   "image/x-icon" },
        { ".html",  "text/html" },
        { ".js",    "text/javascript" },
        { ".json",  "application/json" },
        { ".css",   "text/css" },
        { ".png",   "image/png" },
        { ".jpg",   "image/jpeg" },
        { ".jpeg",  "image/jpeg" },
    };
    if (auto found = map.find(ext); found != map.end())
        return found->second;
    return {};
}

void amber::http::Message::setHeader(std::string_view name, std::string_view value)
{
    if (auto found = m_headers.find(name); found != m_headers.end())
        found->second = value;
    else
        m_headers.emplace(name, value);
}
auto amber::http::Message::getHeader(std::string_view name) -> std::string_view
{
    if (auto found = m_headers.find(name); found != m_headers.end())
        return found->second;
    return {};
}

void amber::http::Message::setBody(std::string_view body)   { m_body.assign(body); }
auto amber::http::Message::getBody() -> std::string_view    { return m_body; }

// GET /hello HTTP/1.1
// Host: localhost:3001
// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:124.0) Gecko/20100101 Firefox/124.0
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
// Accept-Language: en-GB,en;q=0.5
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive
// Upgrade-Insecure-Requests: 1
// Sec-Fetch-Dest: document
// Sec-Fetch-Mode: navigate
// Sec-Fetch-Site: none
// Sec-Fetch-User: ?1

amber::http::Request::Request(std::string_view data)
{
    auto extractMethod = [&](std::string_view str, Method method) -> bool
    {
        if (data.starts_with(str))
        {
            data.remove_prefix(str.length());
            m_method = method;
            return true;
        }
        return false;
    };

    auto extractUntil = [&](char delim)
    {
        auto find = data.find(delim);
        if (find == std::string::npos)
            return data;
        auto tmp = data.substr(0, find);
        data.remove_prefix(find);
        if (data.starts_with(delim))
            data.remove_prefix(1);
        return tmp;
    };

    if (!extractMethod("PUT", Method::put)
        && !extractMethod("POST", Method::post)
        && !extractMethod("GET", Method::get)
        && !extractMethod("DELETE", Method::del))
        return;

    data.remove_prefix(1); // trailing space
    m_path = extractUntil(' ');
    auto newlinePos = data.find('\n');
    data.remove_prefix(newlinePos+1);
    while ((newlinePos = data.find('\n')) != std::string::npos)
    {
        auto headerName = extractUntil(':');
        if (data.starts_with(' '))
            data.remove_prefix(1);
        auto headerValue = extractUntil('\n');
        if (headerValue.ends_with('\r'))
            headerValue.remove_suffix(1);
        if (!headerName.empty() && !headerValue.empty())
            setHeader(headerName, headerValue);
    }
    for (auto& [name, val] : m_headers)
        std::cout << name << ": " << val << '\n';
    std::cout << '\n';
}

auto amber::http::Request::getPath() -> std::string_view { return m_path; }
auto amber::http::Request::getMethod() -> Method { return m_method; }

amber::http::Response::Response()
    : m_status(none)
{}

amber::http::Response::Response(StatusCode status)
    : m_status(status)
{}

// set param includeBody to false only for logging purposes
auto amber::http::Response::toString(bool includeBody /* = true */) -> std::string
{
    auto statusMsg = [&]() -> std::string_view
    {
        switch (m_status)
        {
        case ok_200:        return "OK"sv;          break;
        case notFound_404:  return "Not found"sv;   break;
        default: break;
        }
        std::cout << "unknown status value " << m_status << '\n';
        return {};
    }();
    std::stringstream ss;
    ss << "HTTP/1.1 " << m_status << ' ' << statusMsg << "\n";
    for (auto [key, value] : m_headers)
        ss << key << ": " << value << "\n";
    if (includeBody)
        ss << "\n" << m_body;
    return ss.str();
}
