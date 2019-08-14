//
// Created by timog on 26.05.19.
//

#include "SessionMiddleware.hpp"

namespace webserv
{

std::map<std::string, Session> SessionMiddleware::_sessions;

void SessionMiddleware::process_request(Request& req, Response& res)
{
    try {
        Cookie session_cookie = req.get_cookies().at("_sessid");
        last_uuid = session_cookie.get_value();
        _sessions.at(session_cookie.get_value());
    } catch(std::out_of_range& e) {
        last_uuid = UUID4Generator::instance().generate_uuid4();
        _sessions.emplace(last_uuid, Session("_sessid", last_uuid));
    }

}

void SessionMiddleware::process_response(Request& req, Response& res)
{
    /**if(!m_last_session.isEmpty()) {
        res.addCookie(m_last_session.makeCookie());
    } else {
        //res.addCookie(m_last_session.makeDeleteCookie());
    }*/

    try {
        _sessions.at(last_uuid);
        res.add_cookie(_sessions.at(last_uuid).make_cookie());
    } catch(std::out_of_range& e) {

    }
}

}
