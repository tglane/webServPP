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

/**
 * @brief Class representing a http request
 * Stores data of a http request in member variables
 */
class Request {

public:

    Request() = default;

    /**
     * Parses a given http request into the calling request object structure
     * @param request given request in string form
     */
    void parse(char* request);

    std::map<std::string, std::string> get_headers() { return m_headers; }

    std::string get_header(const string &key);

    std::string get_method() { return m_method; }

    std::string get_resource() { return m_resource; }

    std::string get_protocol() { return m_protocol; }

    std::string get_path() { return m_path; }

    std::map<std::string, std::string> get_params() { return m_params; }

    std::string get_param(const std::string &key);

    std::map<std::string, Cookie> get_cookies() { return m_cookies; }

    Cookie get_cookie(const std::string& cookie_name);

private:

    /**
     * Parses a http request line and stores the parameters in member variables
     * @param requestline String that contains a http request line
     */
    void parse_requestline(const std::string& requestline);

    /**
     * Parses the query string into map m_params
     * @param param_string
     */
    void parse_params(std::string&& param_string);

    /**
     * Constructs cookie objects from a http request header
     * @param cookies http header containing the cookies
     */
    void parse_cookies(const std::string& cookies);

    std::string m_request;    /// unparsed request

    std::string m_method;     /// http method used by this request (e.g. post, get, ...)
    std::string m_protocol;   /// protocol of this request - should be HTTP/*.*
    std::string m_resource;   /// resource addressed by this request
    std::string m_path;       /// path of the resource addressed by this request
    std::string m_fragment; //TODO parse fragment?

    std::map<std::string, std::string> m_params; /// contains names and values of the query string
    std::map<std::string, std::string> m_headers; /// contains names and values of the http request headers
    std::map<std::string, Cookie> m_cookies; /// contains names and values of the cookies

};

#endif //CPPWEBSERVER_REQUEST_HPP