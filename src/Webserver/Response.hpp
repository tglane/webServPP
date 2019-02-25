//
// Created by timog on 23.02.19.
//

#ifndef CPPWEBSERVER_RESPONSE_HPP
#define CPPWEBSERVER_RESPONSE_HPP

#include <socketwrapper/TCPSocket.hpp>
#include <string>
#include <map>
#include <memory>

#include "Cookie.hpp"
#include "Request.hpp"

using std::string;
using std::map;

class Response {

public:

    using Ptr = std::shared_ptr<Response>;

    Response(socketwrapper::TCPSocket::Ptr& conn, Request::Ptr &req) : m_conn(conn), m_req(req) {}

    void addHeader(string key, string value);

    void addCookie(Cookie cookie);

    void setContentType(string contentType);

    void setParams(string code = "", string body = "");

    void send();

    void sendTemplate();

private:

    /**
     * Returns the status phrase to the given http status code
     * @param code http status code
     * @return http status code phrase
     */
    string getPhrase(string code);

    socketwrapper::TCPSocket::Ptr m_conn;   /// Socket connected to the client
    Request::Ptr m_req;                     /// Request from the client to answer with this response

    string m_code;
    string m_body;

    map<string, string> m_headers;
    map<string, Cookie> m_cookies;

};


#endif //CPPWEBSERVER_RESPONSE_HPP
