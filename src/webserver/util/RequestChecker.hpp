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

bool check_request(const Request& req);

}

#endif //CPPWEBSERVER_REQUESTCHECKER_HPP
