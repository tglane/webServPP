//
// Created by timog on 05.08.19.
//

#include "AuthenticationService.hpp"

AuthenticationService& AuthenticationService::get_instance()
{
    static AuthenticationService auth;
    return auth;
}

bool AuthenticationService::authenticate(const std::string &identifier, const std::string &password)
{
    size_t password_hash = std::hash<std::string> {}(password);
}
