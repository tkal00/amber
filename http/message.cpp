#include "message.h"
#include <iostream>

void amber::http::Message::setHeader(std::string_view name, std::string_view value)
{
    auto found = m_headers.lower_bound(name);
    if (found != m_headers.end() && m_headers.key_comp()(found->first, name))
        found->second = value;
    else
        m_headers.emplace_hint(found, name, value);
}

void amber::http::Message::setBody(std::string_view body)
{
    m_body = body;
}

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
    m_isValid = true;
}
