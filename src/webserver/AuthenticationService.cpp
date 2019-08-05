//
// Created by timog on 05.08.19.
//

#include "AuthenticationService.hpp"

AuthenticationService& AuthenticationService::get_instance()
{
    static AuthenticationService auth;
    return auth;
}

Result AuthenticationService::authenticate(const std::string &identifier, const std::string &password, AuthenticationAdapter &auth)
{
    size_t password_hash = std::hash<std::string> {}(password);
    return this->authenticate(identifier, password_hash, auth);
}

Result AuthenticationService::authenticate(const std::string &identifier, size_t password_hash, AuthenticationAdapter& auth)
{
    return auth.authenticate_user(identifier, password_hash);
}
