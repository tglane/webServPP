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
    m_status = code;
    m_body = body;
}

void Response::send()
{
    char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title><body><h1>Goodbye, world!</h1><form id=\"main_form\" method=\"post\"><textarea>Hallo</textarea id=\"id_text\"><input type=\"submit\"></form></body></html>\r\n";
    m_conn->write(response);
}

void Response::sendTemplate()
{

}