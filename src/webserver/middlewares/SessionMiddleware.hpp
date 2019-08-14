//
// Created by timog on 26.05.19.
//

#ifndef WEBSERV_PP_SESSIONMIDDLEWARE_HPP
#define WEBSERV_PP_SESSIONMIDDLEWARE_HPP

#include <map>

#include "Middleware.hpp"
#include "../Session.hpp"
#include "../Cookie.hpp"

namespace webserv
{

/**
 * @brief Checks for Session Cookies in incoming requests and puts them into the outgoing response
 */
class SessionMiddleware : public Middleware {

public:

    void process_request(Request& req, Response& res) override;

    void process_response(Request& req, Response& res) override;

private:

    std::string last_uuid;

    static std::map<std::string, Session> _sessions;

};

}

#endif //WEBSERV_PP_SESSIONMIDDLEWARE_HPP
