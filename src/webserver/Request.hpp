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

namespace webserv {
    using std::string;

/**
* @brief Class representing a http request
* Stores data of a http request in member variables
*/
class Request {

public:

    Request() = default;

    /**
     * Directly creates a request object from a given request in string representation
     * @param request_string string containing an http request
     */
    explicit Request(const char *request_string) { this->parse(request_string); }

    /**
     * Parses a given http request into the calling request object structure
     * @param request given request in string form
     */
    void parse(const char *request);

    /**
     * @brief Returns a string representation of the request object
     * @return
     */
    std::string create_string();

    /** Getter and setter methods **/
    std::map<std::string, std::string> get_headers() { return m_headers; }
    std::string get_header(const string &key) { return get_from_map(m_headers, key); };
    void set_header(const string& key, const string& value) { m_headers[key] = value; };

    std::string get_method() { return m_method; }
    void set_method(const string& method) { m_method = method; };

    std::string get_resource() { return m_resource; }
    void set_resource(const string& resource) { m_resource = resource; };

    std::string get_protocol() { return m_protocol; }
    void set_protocol(const string& protocol) { m_protocol = protocol; };

    std::string get_path() { return m_path; }
    void set_path(const string& path) { m_path = path; };

    std::map<std::string, std::string> get_params() { return m_query_params; }
    std::string get_param(const std::string &key) { return get_from_map(m_query_params, key); };
    void set_param(const string& key, const string& value) { m_query_params[key] = value; };

    std::map<std::string, Cookie> get_cookies() { return m_cookies; }
    Cookie get_cookie(const std::string &cookie_name);
    void set_cookie(const string& key, const Cookie& cookie) { m_cookies[key] = cookie; };

    std::map<std::string, std::string> get_post_params() { return m_body_params; }
    std::string get_post_param(const std::string &key) { return get_from_map(m_body_params, key); };
    void set_post_param(const string& key, const string& value) { m_body_params[key] = value; };

private:

    /**
     * Parses a http request line and stores the parameters in member variables
     * @param requestline String that contains a http request line
     */
    void parse_requestline(const std::string &requestline);

    /**
     * Parses the query string into map m_params
     * @param param_container map to search
     * @param param_string
     */
    static void parse_params(std::string &&param_string, std::map<std::string, std::string> &param_container);

    /**
     * Constructs cookie objects from a http request header
     * @param cookies http header containing the cookies
     */
    void parse_cookies(const std::string &cookies);

    /**
     * Helper function
     */
    static std::string get_from_map(const std::map<std::string, std::string> &container, const std::string &key);

    std::string m_request;    /// unparsed request

    std::string m_method;     /// http method used by this request (e.g. post, get, ...)
    std::string m_protocol;   /// protocol of this request - should be HTTP/*.*
    std::string m_resource;   /// resource addressed by this request
    std::string m_path;       /// path of the resource addressed by this request

    std::map<std::string, std::string> m_query_params; /// contains names and values of the query string
    std::map<std::string, std::string> m_headers; /// contains names and values of the http request headers
    std::map<std::string, Cookie> m_cookies; /// contains names and values of the cookies
    std::map<std::string, std::string> m_body_params; /// Contains post params from the requests body

};

}

#endif //CPPWEBSERVER_REQUEST_HPP
