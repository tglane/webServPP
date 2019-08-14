//
// Created by timog on 16.04.19.
//

#ifndef CPPWEBSERVER_REQUESTCHECKER_HPP
#define CPPWEBSERVER_REQUESTCHECKER_HPP

#include <regex>
#include "../Request.hpp"

namespace webserv
{

/**
 * @brief Checks a given http request for validity
 * Implemented as singleton > use getInstance() to create/get an instance of this class
 */
class RequestChecker {

public:

    RequestChecker() = default;

    RequestChecker(const RequestChecker&) = delete;
    void operator=(const RequestChecker&) = delete;

    bool check_request(Request& req);

private:

    bool check_requestline(const string& method, const string& resource, const string& protocol);

    bool check_host(Request& req);

};

}

#endif //CPPWEBSERVER_REQUESTCHECKER_HPP
