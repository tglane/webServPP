//
// Created by timog on 30.03.19.
//

#ifndef CPPWEBSERVER_LOGGINGMIDDLEWARE_HPP
#define CPPWEBSERVER_LOGGINGMIDDLEWARE_HPP

#include "Middleware.hpp"

/**
 * @brief Middleware to log information of incoming requests and generated responses to the console
 */
class LoggingMiddleware : public Middleware {

public:

    void process_request(Request& req, Response& res) override;

    void process_response(Request& req, Response& res) override;

};

#endif //CPPWEBSERVER_LOGGINGMIDDLEWARE_HPP
