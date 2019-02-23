//
// Created by timog on 23.02.19.
//

#ifndef CPPWEBSERVER_RESPONSE_HPP
#define CPPWEBSERVER_RESPONSE_HPP

#include <socketwrapper/TCPSocket.hpp>
#include <string>
#include <map>
#include <memory>

using std::string;
using std::map;

class Response {

public:

    using Ptr = std::shared_ptr<Response>;

    Response(socketwrapper::TCPSocket::Ptr& conn) : m_conn(conn) {}

    void addHeader(string key, string value);

    //void addCookie(Cookie cookie);

    void setContentType(string contentType);

    void setParams(string code = "", string body = "");

    void send();

    void sendTemplate();

private:

    socketwrapper::TCPSocket::Ptr m_conn;

    string m_status;
    string m_body;

    map<string, string> m_headers;

    //Cookie m_cookies[]; -> must be implemented first

};


#endif //CPPWEBSERVER_RESPONSE_HPP
