//
// Created by timog on 27.05.19.
//

#include "Session.hpp"

namespace webserv
{

Session::Session(std::string cookiename, std::string sessid)
{
    if(!sessid.empty())
    {
        m_sessid = std::move(sessid);
    }
    else
    {
        m_sessid = generate_uuid4();
    }
    m_cookiename = std::move(cookiename);
}

bool Session::is_empty()
{
    return (m_sessid.empty() && m_cookiename.empty());
}

void Session::renew_session()
{
    m_sessid = generate_uuid4();
    m_data.clear();
}

void Session::destroy_session()
{
    m_sessid = "";
    m_data.clear();
}

string Session::getDataItem(const std::string &key)
{
    try
    {
        return m_data.at(key);
    }
    catch(std::out_of_range& e)
    {
        return "";
    }
}

}
