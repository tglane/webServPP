//
// Created by timog on 27.05.19.
//

#ifndef WEBSERV_PP_SESSION_HPP
#define WEBSERV_PP_SESSION_HPP

#include <string>
#include <map>

#include "util/UUID4Generator.hpp"
#include "Cookie.hpp"

namespace webserv {

/**
* @brief Representation of a HTTP Session Cookie
* Using UUID4 for generating session IDs
*/
class Session {

public:

    explicit Session(std::string cookiename = "_sessid", std::string sessid = "");

    /**
     * @brief Creates a cookie representing the session object
     * @return Cookie
     */
    Cookie make_cookie() { return Cookie{m_cookiename, m_sessid}; }

    /**
     * @brief Creates a cookie with expiry date in the past to destroy the cookie containing the session id at the client
     * @return Cookie
     */
    Cookie make_delete_cookie() { return Cookie{m_cookiename, m_sessid, false, false, "", "", "", "", -1}; }

    bool is_empty();

    /**
     * @brief Generates a new Session from an existing session object
     */
    void renew_session();

    /**
     * @brief Destroys the session by clearing the session id and the session data
     */
    void destroy_session();

    /**
     * @brief Returns some data element or empty string
     * @param key
     * @return string containing the data element
     */
    std::string getDataItem(const std::string &key);

    /**
     * @brief Returns the complete map containing the session data
     * @return map
     */
    std::map<std::string, std::string> getData() { return m_data; }

private:

    std::string m_cookiename;                /// Name of the cookie for storing session IDs
    std::string m_sessid;                    /// Secret session ID -> UUID4
    std::map<std::string, std::string> m_data;    /// Stores the session data

};

}

#endif //WEBSERV_PP_SESSION_HPP
