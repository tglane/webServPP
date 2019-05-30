//
// Created by timog on 27.05.19.
//

#ifndef WEBSERV_PP_SESSION_HPP
#define WEBSERV_PP_SESSION_HPP

#include <string>
#include <map>

#include "util/UUID4Generator.hpp"
#include "Cookie.hpp"

using std::string;
using std::map;

/**
 * @brief Representation of a HTTP Session Cookie
 * Using UUID4 for generating session IDs
 */
class Session {

public:

    explicit Session(string cookiename = "_sessid", string sessid = "");

    /**
     * @brief Creates a cookie representing the session object
     * @return Cookie
     */
    Cookie makeCookie() { return Cookie{m_cookiename, m_sessid}; }

    Cookie makeDeleteCookie() { return Cookie{m_cookiename, m_sessid, false, false, "", "", "", "", -1}; }

    bool isEmpty();

    /**
     * Returns some data element or empty string
     * @param key
     * @return
     */
    string getDataItem(const string& key);

    map<string, string> getData() { return m_data; }

private:

    /**
     * @brief Generates a new Session from an existing session object
     */
    void renewSession();

    string m_cookiename;                /// Name of the cookie for storing session IDs
    string m_sessid;                    /// Secret session ID -> UUID4
    std::map<string, string> m_data;    /// Stores the session data

};

#endif //WEBSERV_PP_SESSION_HPP
