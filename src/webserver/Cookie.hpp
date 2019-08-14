//
// Created by timog on 24.02.19.
//

#ifndef CPPWEBSERVER_COOKIE_HPP
#define CPPWEBSERVER_COOKIE_HPP

#include <string>
#include <map>
#include <memory>
#include <chrono>
#include <iostream>
#include <ctime>

namespace webserv
{

using std::string;

/**
 * @brief Representation of a http cookie to send to a client
 * Stores all information of a cookie in member variables
 */
class Cookie {

public:

    /// Constructor
    Cookie(string name, string value, bool httpOnly = false, bool secure = false, string comment = "",
        string domain = "", string max_age = "",string path = "/", int expires = 0);

    /**
     * Creates a string representation of the member fields to set this cookie
     * @return string representation of this cookie
     */
    string build_header();

    void set_expiry_date(int days);

    void set_http_only(bool value) { m_http_only = value; }

    void set_secure(bool value) { m_secure = value; }

    string get_name() { return m_name; }

    string get_value() { return m_value; }

private:

    /**
     * Parameters of a http cookie
     * Not all headers are to set
     */
    string m_name;
    string m_value;
    bool m_http_only;
    bool m_secure;
    string m_comment;
    string m_domain;
    string m_max_age;
    string m_path;
    string m_expires;

};

}

#endif //CPPWEBSERVER_COOKIE_HPP
