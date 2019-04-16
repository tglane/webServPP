//
// Created by timog on 16.04.19.
//

#ifndef CPPWEBSERVER_REQUESTCHECKER_HPP
#define CPPWEBSERVER_REQUESTCHECKER_HPP

#include "../Request.hpp"

/**
 * @brief Checks a given http request for validity
 * Implemented as singleton > use getInstance() to create/get an instance of this class
 */
class RequestChecker {

public:

    RequestChecker() = default;

    RequestChecker(const RequestChecker&) = delete;
    void operator=(const RequestChecker&) = delete;

    bool checkRequest(Request& req);

private:

    bool checkRequestline(const string& method, const string& resource, const string& protocol);

};


#endif //CPPWEBSERVER_REQUESTCHECKER_HPP
