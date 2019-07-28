//
// Created by timog on 26.05.19.
//

#ifndef WEBSERV_PP_SESSIONMIDDLEWARE_HPP
#define WEBSERV_PP_SESSIONMIDDLEWARE_HPP

#include <map>

#include "Middleware.hpp"
#include "../Session.hpp"
#include "../Cookie.hpp"

using std::map;
using std::string;

/**
 * @brief Checks for Session Cookies in incoming requests and puts them into the outgoing response
 */
class SessionMiddleware : public Middleware {

public:

    void processRequest(Request& req, Response& res) override;

    void processResponse(Response& res) override;

private:

    string last_uuid;

    static map<string, Session> c_sessions;

};


#endif //WEBSERV_PP_SESSIONMIDDLEWARE_HPP
