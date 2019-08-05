//
// Created by timog on 05.08.19.
//

#ifndef WEBSERV_PP_AUTHENTICATIONSERVICE_HPP
#define WEBSERV_PP_AUTHENTICATIONSERVICE_HPP

#include "Request.hpp"
#include "Response.hpp"
#include <string>

class AuthenticationAdapter {

public:



protected:

    AuthenticationAdapter()  = default;

private:

};

/**
 * @brief Class for user authentication implemented as singleton
 *  Get instance via get_instance() function and then call authenticate()
 */
class AuthenticationService {

public:

    static AuthenticationService& get_instance();

    /**
     * Delete copy constructor and operator = to avoid copies of singleton instance
     */
    AuthenticationService& operator= (const AuthenticationService&) = delete;
    AuthenticationService(const AuthenticationService&) = delete;

    /**
     * @brief Authenticates a user
     * @param identifier string with identifier of user; e.g. username, name or email
     * @param password string with password to check
     * @return bool
     */
    bool authenticate(const std::string& identifier, const std::string& password);

private:

    /// Private constructor to call from get_instance function only
    AuthenticationService() = default;

};


#endif //WEBSERV_PP_AUTHENTICATIONSERVICE_HPP
