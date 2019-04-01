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

    void processRequest(Request::Ptr req, Response::Ptr res) override;

    void processResponse(Response::Ptr res) override;

};

#endif //CPPWEBSERVER_LOGGINGMIDDLEWARE_HPP
