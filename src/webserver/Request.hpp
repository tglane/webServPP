//
// Created by timog on 19.02.19.
//

#ifndef CPPWEBSERVER_REQUEST_HPP
#define CPPWEBSERVER_REQUEST_HPP

#include <string>
#include <sstream>
#include <istream>
#include <iterator>
#include <map>
#include <vector>
#include <memory>

#include "Cookie.hpp"

using std::string;
using std::map;

/**
 * @brief Class representing a http request
 * Stores data of a http request in member variables
 */
class Request {

public:
    using Ptr = std::shared_ptr<Request>;

    Request() = default;

    /**
     * Parses a given http request into the calling request object structure
     * @param request given request in string form
     */
    void parse(char* request);

    /**
     * @brief Checks wether the request object represents a valid http request
     * Should be called after call parse() method
     * @return
     */
    bool isValid();

    map<string, string> getHeaders() { return m_headers; }

    string getMethod() { return m_method; }

    string getProtocol() { return m_protocol; }

    string getPath() { return m_path; }

    std::map<string, string> getParams() { return m_params; }

    std::map<string, Cookie> getCookies() { return m_cookies; }

private:

    /**
     * Parses a http request line and stores the parameters in member variables
     * @param requestline String that contains a http request line
     */
    void parse_requestline(string requestline);

    /**
     * Parses the query string into map m_params
     * @param param_string
     */
    void parse_params(string param_string);

    /**
     * Constructs cookie objects from a http request header
     * @param cookies http header containing the cookies
     */
    void parse_cookies(string cookies);

    string m_request;    /// unparsed request

    string m_method;     /// http method used by this request (e.g. post, get, ...)
    string m_protocol;   /// protocol of this request - should be HTTP/*.*
    string m_resource;   /// resource addressed by this request
    string m_path;       /// path of the resource addressed by this request
    string m_fragment;

    map<string, string> m_params; /// contains names and values of the query string
    map<string, string> m_headers; /// contains names and values of the http request headers
    map<string, Cookie> m_cookies; /// contains names and values of the cookies

};

#endif //CPPWEBSERVER_REQUEST_HPP
