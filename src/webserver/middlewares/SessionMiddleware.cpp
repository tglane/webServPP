//
// Created by timog on 26.05.19.
//

#include "SessionMiddleware.hpp"

map<string, Session> SessionMiddleware::c_sessions;

void SessionMiddleware::processRequest(Request& req, Response& res)
{
    try {
        Cookie session_cookie = req.get_cookies().at("_sessid");
        last_uuid = session_cookie.get_value();
        c_sessions.at(session_cookie.get_value());
    } catch(std::out_of_range& e) {
        last_uuid = UUID4Generator::instance().generate_uuid4();
        c_sessions.emplace(last_uuid, Session("_sessid", last_uuid));
    }

}

void SessionMiddleware::processResponse(Response& res)
{
    /**if(!m_last_session.isEmpty()) {
        res.addCookie(m_last_session.makeCookie());
    } else {
        //res.addCookie(m_last_session.makeDeleteCookie());
    }*/

    try {
        c_sessions.at(last_uuid);
        res.add_cookie(c_sessions.at(last_uuid).makeCookie());
    } catch(std::out_of_range& e) {

    }
}
