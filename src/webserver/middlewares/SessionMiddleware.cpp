//
// Created by timog on 26.05.19.
//

#include "SessionMiddleware.hpp"

map<string, Session> SessionMiddleware::c_sessions;

void SessionMiddleware::processRequest(Request::Ptr req, Response::Ptr res)
{
    try {
        Cookie session_cookie = req->getCookies().at("_sessid");
        last_uuid = session_cookie.getValue();
        c_sessions.at(session_cookie.getValue());
    } catch(std::out_of_range& e) {
        last_uuid = UUID4Generator::instance().generateUUID4();
        c_sessions.emplace(last_uuid, Session("_sessid", last_uuid));
    }

}

void SessionMiddleware::processResponse(Response::Ptr res)
{
    /**if(!m_last_session.isEmpty()) {
        res->addCookie(m_last_session.makeCookie());
    } else {
        //res->addCookie(m_last_session.makeDeleteCookie());
    }*/

    try {
        c_sessions.at(last_uuid);
        res->addCookie(c_sessions.at(last_uuid).makeCookie());
    } catch(std::out_of_range& e) {

    }
}
