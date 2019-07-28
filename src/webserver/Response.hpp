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


using std::string;
using std::map;
using std::list;

/**
 * @brief Class representing a http response
 * Stores data of a http response in member variables and sends them to a client
 */
class Response {

public:
    /// Constructor
    explicit Response(std::shared_ptr<Request> req) : m_req {std::move(req)}
    {}

    /**
     * @brief Creates http response from member of the objects
     * Creates the response line, the header fields and the response body
     * @return string representation of the response
     */
    string create_string();

    /**
     * @brief Reads a template file and substitutes its placeholders to build the http body and sets m_body
     * @param templateFile filename of the html template located in src/templates
     * @param values map containing strings as placeholders in the template to replace with strings or lists of strings
     *          from this map
     */
    void set_body_from_template(const string& templateFile, const map<string, std::variant<string, int, list<string>>>& values);

    /**
     * @brief Reads a file from given filename and uses it as the response body
     * @param bodyFile filename
     */
    void set_body_from_file(const string& bodyFile);

    /**
     * @brief Adds header Location: url, sets status to 302 and calls method send() to send a redirect to url
     * @param url to redirect to
     */
    void send_redirect(const string& url);

    /**
     * Adds a header to m_headers with key: value for the response
     * @param key
     * @param value
     */
    void add_header(const string& key, const string& value);

    /**
     * Adds a cookie to m_cookies with name: cookie
     * @param cookie
     */
    void add_cookie(Cookie cookie);

    /**
     * Sets the header field Content-Type to the given string
     */
    void set_content_type(const string& contentType);

    void set_code(string code) { m_code = std::move(code); }

    void set_body(const string& body);

    string get_code() { return m_code; }

private:

    /**
     * Returns the status phrase to the given http status code
     * @param code http status code
     * @return http status code phrase
     */
    string get_phrase(const string& code);

    std::shared_ptr<Request> m_req;                     /// Request from the client to answer with this response

    string m_code;
    string m_body;

    map<string, string> m_headers;
    map<string, Cookie> m_cookies;

    static std::mutex c_file_mutex;

};

#endif //CPPWEBSERVER_RESPONSE_HPP
