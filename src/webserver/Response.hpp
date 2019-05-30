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

    using Ptr = std::shared_ptr<Response>;

    /// Constructor
    Response(Request::Ptr &req) : m_req(req) {}

    /**
     * @brief Creates http response from member of the objects
     * Creates the response line, the header fields and the response body
     * @return string representation of the response
     */
    string createString();

    /**
     * @brief Reads a template file and substitutes its placeholders to build the http body and sets m_body
     * @param templateFile filename of the html template located in src/templates
     * @param values map containing strings as placeholders in the template to replace with strings or lists of strings
     *          from this map
     */
    void setBodyFromTemplate(const string& templateFile, map<string, std::variant<string, int, list<string>>> values);

    /**
     * @brief Reads a file from given filename and uses it as the response body
     * @param bodyFile filename
     */
    void setBodyFromFile(const string& bodyFile);

    /**
     * @brief Adds header Location: url, sets status to 302 and calls method send() to send a redirect to url
     * @param url to redirect to
     */
    void sendRedirect(const string& url);

    /**
     * Adds a header to m_headers with key: value for the response
     * @param key
     * @param value
     */
    void addHeader(const string& key, const string& value);

    /**
     * Adds a cookie to m_cookies with name: cookie
     * @param cookie
     */
    void addCookie(Cookie cookie);

    /**
     * Sets the header field Content-Type to the given string
     */
    void setContentType(const string& contentType);

    void setCode(string code) { m_code = std::move(code); }

    void setBody(const string& body);

    string getCode() { return m_code; }

private:

    /**
     * Returns the status phrase to the given http status code
     * @param code http status code
     * @return http status code phrase
     */
    string getPhrase(const string& code);

    Request::Ptr m_req;                     /// Request from the client to answer with this response

    string m_code;
    string m_body;

    map<string, string> m_headers;
    map<string, Cookie> m_cookies;

    static std::mutex mu;

};

#endif //CPPWEBSERVER_RESPONSE_HPP
