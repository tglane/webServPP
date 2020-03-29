//
// Created by timog on 23.02.19.
//

#ifndef CPPWEBSERVER_RESPONSE_HPP
#define CPPWEBSERVER_RESPONSE_HPP

#include <socketwrapper/TCPSocket.hpp>
#include <string>
#include <map>
#include <memory>
#include <list>
#include <variant>
#include <exception>
#include <mutex>

#include "Cookie.hpp"
#include "Request.hpp"
#include "util/Statuscodes.hpp"

/**
 * @brief Class representing a http response
 * Stores data of a http response in member variables and sends them to a client
 */
class Response {

public:
    /// Constructor
    explicit Response(const Request& req) : m_req(req)
    {}

    /**
     * @brief Creates http response from member of the objects
     * Creates the response line, the header fields and the response body
     * @return string representation of the response
     */
    std::string create_string();

    /**
     * @brief Reads a template file and substitutes its placeholders to build the http body and sets m_body
     * @param templateFile filename of the html template located in src/templates
     * @param values map containing strings as placeholders in the template to replace with strings or lists of strings
     *          from this map
     */
    void set_body_from_template(const std::string& templateFile, 
        const std::map<std::string, std::variant<std::string, int, std::list<std::string>>>& values);

    /**
     * @brief Reads a file from given filename and uses it as the response body
     * @param bodyFile filename
     */
    void set_body_from_file(const std::string& bodyFile);

    /**
     * @brief Adds header Location: url, sets status to 302 and calls method send() to send a redirect to url
     * @param url to redirect to
     */
    void send_redirect(const std::string& url);

    /**
     * Adds a header to m_headers with key: value for the response
     * @param key
     * @param value
     */
    void add_header(const std::string& key, const std::string& value);

    /**
     * Adds a cookie to m_cookies with name: cookie
     * @param cookie
     */
    void add_cookie(Cookie&& cookie);

    /**
     * Sets the header field Content-Type to the given string
     */
    void set_content_type(const std::string& contentType);

    void set_code(int code) { m_code = code; }

    void set_body(const std::string& body);

    int get_code() { return m_code; }

private:

    /**
     * Returns the status phrase to the given http status code
     * @param code http status code
     * @return http status code phrase
     */
    std::string get_phrase(int code);

    Request m_req;                     /// Request from the client to answer with this response

    int m_code = 0;
    std::string m_body;

    std::map<std::string, std::string> m_headers;
    std::map<std::string, Cookie> m_cookies;

    static std::mutex c_file_mutex;

};

#endif //CPPWEBSERVER_RESPONSE_HPP

