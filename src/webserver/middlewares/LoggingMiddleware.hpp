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

    void processRequest(Request& req, Response& res) override;

    void processResponse(Response& res) override;

};

#endif //CPPWEBSERVER_LOGGINGMIDDLEWARE_HPP
