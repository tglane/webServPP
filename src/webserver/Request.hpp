//
// Created by timog on 19.02.19.
//

#ifndef CPPWEBSERVER_REQUEST_HPP
#define CPPWEBSERVER_REQUEST_HPP

#include <string>
#include <string_view>
#include <sstream>
#include <istream>
#include <iterator>
#include <map>
#include <vector>
#include <memory>

#include "Cookie.hpp"

namespace webserv {

/**
* @brief Class representing a http request
* Stores data of a http request in member variables
*/
class Request {

public:

    /** 
     * Delete default constructor because a completely empty http request makes no sense at all
     */
    Request() = default;

    /**
     * Directly creates a request object from a given request in string representation
     * @param request_string string containing an http request
     */
    explicit Request(const char* request_string);

    Request(const Request& other) = default;

    Request(Request&& other) noexcept;

    Request& operator=(const Request& other) = default;

    Request& operator=(Request&& other);

    /**
     * Parses a given http request into the calling request object structure
     * @param request given request in string form
     */
    void parse(const char *request);

    /**
     * @brief Returns a string representation of the request object
     * @return
     */
    std::string to_string() const;

    std::map<std::string_view, std::string_view> get_headers() const { return m_headers; }

    std::string get_header(const std::string& key) const;

    std::string get_method() const { return std::string(m_method); }

    std::string get_resource() const { return std::string(m_resource); }

    std::string get_protocol() const { return std::string(m_protocol); }

    std::string get_path() const { return std::string(m_path); }

    std::map<std::string_view, std::string_view> get_params() const { return m_query_params; }

    std::string get_param(const std::string& key) const;

    std::map<std::string, Cookie> get_cookies() const { return m_cookies; }

    Cookie get_cookie(const std::string &cookie_name) const;

    std::map<std::string_view, std::string_view> get_post_params() const { return m_body_params; }

    std::string get_post_param(const std::string &key) const;

private:

    /**
     * Parses a http request line and stores the parameters in member variables
     * @param requestline String that contains a http request line
     */
    void parse_requestline(std::string_view requestline);

    /**
     * Parses the query string into map m_params
     * @param param_container map to search
     * @param param_string
     */
    static void parse_params(std::string_view param_string, std::map<std::string_view, std::string_view>& param_container);

    /**
     * Constructs cookie objects from a http request header
     * @param cookies http header containing the cookies
     */
    void parse_cookies(const std::string &cookies);

    std::string m_request;    /// unparsed request

    std::string_view m_method;     /// http method used by this request (e.g. post, get, ...)
    std::string_view m_protocol;   /// protocol of this request - should be HTTP/*.*
    std::string_view m_resource;   /// resource addressed by this request
    std::string_view m_path;       /// path of the resource addressed by this request
    std::string_view m_fragment;   /// TODO parse fragment?!

    std::map<std::string_view, std::string_view> m_query_params; /// contains names and values of the query string
    std::map<std::string_view, std::string_view> m_headers; /// contains names and values of the http request headers
    std::map<std::string, Cookie> m_cookies; /// contains names and values of the cookies
    std::map<std::string_view, std::string_view> m_body_params; /// Contains post params from the requests body

};

}

#endif //CPPWEBSERVER_REQUEST_HPP

