//
// Created by timog on 19.02.19.
//

#ifndef CPPWEBSERVER_WEBSERVER_HPP
#define CPPWEBSERVER_WEBSERVER_HPP

#include <memory>
#include <socketwrapper/TCPSocket.hpp>

#include "Request.hpp"
#include "Response.hpp"

/**
 * @brief Simple Webserver that parses a http request and generates a response
 */
class Webserver {

public:

    /**
     * @brief Constructor
     * @param port to listen on
     * @param queue_size for the listen(int queue_size) of the internal tcp socket
     */
    Webserver(int port, int queue_size);

    /**
     * @brief Destructor
     */
    ~Webserver();

    /**
     * @brief starts the main loop of the webserver
     */
    void serve();

private:

    socketwrapper::TCPSocket::Ptr m_socket;

};


#endif //CPPWEBSERVER_WEBSERVER_HPP
