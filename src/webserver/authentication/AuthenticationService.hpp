//
// Created by timog on 05.08.19.
//

#ifndef WEBSERV_PP_AUTHENTICATIONSERVICE_HPP
#define WEBSERV_PP_AUTHENTICATIONSERVICE_HPP

#include "../Request.hpp"
#include "../Response.hpp"
#include <string>

namespace webserv
{

struct Result {
    enum _result_code { SUCCESS, FAILURE, IDENTITY_NOT_FOUND };

    Result(int code, std::string identity)
        : m_code {code}, m_identiy {std::move(identity)} {}

    int get_code() { return m_code; }

private:
    int m_code;
    std::string m_identiy;
};

/**
 * @brief Abstract authentication adapter implemented by authenticator classes which provide stored user identifier and passwords
 */
class AuthenticationAdapter {

public:

    /**
     * @brief authenticates a user by comparing his identifier and password hash with login data provided by an implementation of this class
     * @return Result of authentication process
     */
    virtual Result authenticate_user(const std::string& identifier, size_t password_hash) = 0;

protected:

    AuthenticationAdapter()  = default;

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
    Result authenticate(const std::string& identifier, const std::string& password, AuthenticationAdapter& auth);

    Result authenticate(const std::string& identifier, size_t password_hash, AuthenticationAdapter& auth);

private:

    /// Private constructor to call from get_instance function only
    AuthenticationService() = default;

};

}

#endif //WEBSERV_PP_AUTHENTICATIONSERVICE_HPP
