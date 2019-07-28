//
// Created by timog on 05.03.19.
//

#ifndef CPPWEBSERVER_MIDDELWARE_HPP
#define CPPWEBSERVER_MIDDELWARE_HPP

#include "../Request.hpp"
#include "../Response.hpp"

/**
 * Abstract class for the implementation of middelwares called from the webservers main loop
 */
class Middleware {

public:

    /**
     * @brief Processes the given response in a specific way for each middelware
     * @param req
     * @param res
     */
    virtual void processRequest(Request& req, Response& res) = 0;

    /**
     * @brief Processes the given response in a specific way for each middelware before being send to the client
     * @param res
     */
    virtual void processResponse(Response& res) = 0;

};


#endif //CPPWEBSERVER_MIDDELWARE_HPP
