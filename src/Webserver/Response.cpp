//
// Created by timog on 23.02.19.
//

#include "Response.hpp"
#include <iostream>

void Response::send()
{
    string response;

    /* Set some response fields if missing */
    if(m_code.empty())
    {
        m_code = "200";
    }
    if(m_headers.find("Content-Type") == m_headers.end())
    {
        m_headers.insert(std::pair<string, string>("Content-Type", "text/html; charset=UTF-8"));
    }
    std::time_t now(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

    /* Begin with response line */
    response.append("HTTP/1.1 " + m_code + " " + getPhrase(m_code) + "\r\n" + "Date: " + std::ctime(&now));

    /* Append all cookies to response */
    for(auto it = m_cookies.begin(); it != m_cookies.end(); it++)
    {
        response.append((*it).second.buildHeader() + "\r\n");
    }

    /* Append all headers to response */
    for(auto it = m_headers.begin(); it != m_headers.end(); it++)
    {
        response.append(it->first + ": " + it->second + "\r\n");
    }

    /* Append body to response line */
    response.append("\r\n");
    response.append(m_body);

    /* Convert resonse to char* and send it to the client */
    char* res_arr = new char[response.length()+1];
    strcpy(res_arr, response.c_str());
    std::cout << response << std::endl;
    m_conn->write(res_arr);
    delete[] res_arr;
}

void Response::sendTemplate()
{

}

void Response::addHeader(string key, string value)
{
    auto it = m_headers.find(key);
    if(it != m_headers.end())
    {
        it->second = value;
    }
    else
    {
        m_headers.insert(std::pair<string, string>(key, value));
    }
}

void Response::addCookie(Cookie cookie)
{
    m_cookies.insert(std::pair<string, Cookie>(cookie.getName(), cookie));
}

void Response::setContentType(string contentType)
{
    auto it = m_headers.find("Content-Type");
    if(it != m_headers.end())
    {
        it->second = contentType;
    }
    else
    {
        m_headers.insert(std::pair<string, string>("Content-Type", contentType));
    }
}

string Response::getPhrase(string code)
{
    return "OK";
}