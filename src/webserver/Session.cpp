//
// Created by timog on 27.05.19.
//

#include "Session.hpp"

Session::Session(string cookiename, string sessid)
{
    if(!sessid.empty()) {
        m_sessid = std::move(sessid);
    } else {
        m_sessid = UUID4Generator::instance().generateUUID4();
    }
    m_cookiename = std::move(cookiename);
}

bool Session::isEmpty()
{
    return (m_sessid.empty() && m_cookiename.empty());
}

string Session::getDataItem(const string &key)
{
    try {
        return m_data.at(key);
    } catch(std::out_of_range& e) {
        return "";
    }
}

void Session::renewSession()
{
    m_sessid = UUID4Generator::instance().generateUUID4();
    m_data.clear();
}
