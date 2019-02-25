//
// Created by timog on 23.02.19.
//

#include "Response.hpp"
#include <iostream>

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

void Response::setParams(string code, string body)
{
    m_code = std::move(code);
    m_body = std::move(body);
}

void Response::send()
{
    //TODO !!!
    char body[] = "<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title><body><h1>Goodbye, world!</h1><form id=\"main_form\" method=\"post\"><textarea>Hallo</textarea id=\"id_text\"><input type=\"submit\"></form></body></html>\r\n";
    char head[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
    string response;

    if(m_code.empty())
    {
        m_code = "200";
    }
    Cookie c("testname", "Testvalueeee");
    c.setExpiryDate(10);
    c.setHttpOnly(true);
    std::cout << c.buildHeader() << std::endl;
    response.append("HTTP/1.1 " + m_code + " " + getPhrase(m_code) + "\r\n");
    response.append(c.buildHeader() + "\r\n");
    response.append("Content-Type: text/html; charset=UTF-8\r\n\r\n");
    string s(body);
    response.append(s);

    char* res_arr = new char[response.length()+1];
    strcpy(res_arr, response.c_str());
    m_conn->write(res_arr);
    delete[] res_arr;
}

void Response::sendTemplate()
{

}

string Response::getPhrase(string code)
{
    return "OK";
}
