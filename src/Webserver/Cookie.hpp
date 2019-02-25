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

using std::string;

class Cookie {

public:

    using Ptr = std::shared_ptr<Cookie>;

    Cookie(string name, string value, bool httpOnly = false, bool secure = false, string comment = "",
        string domain = "", string max_age = "",string path = "", int expires = 0);

    string buildHeader();

    void setExpiryDate(int days);

    void setHttpOnly(bool value) { m_httpOnly = value; }

    void setSecure(bool value) { m_secure = value; }

    string getName() { return m_name; }

private:

    string m_name;
    string m_value;
    bool m_httpOnly;
    bool m_secure;
    string m_comment;
    string m_domain;
    string m_max_age;
    string m_path;
    string m_expires;

};


#endif //CPPWEBSERVER_COOKIE_HPP
