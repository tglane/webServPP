//
// Created by timog on 19.02.19.
//

#ifndef CPPWEBSERVER_WEBSERVER_HPP
#define CPPWEBSERVER_WEBSERVER_HPP

#include <memory>
#include <list>
#include <socketwrapper/TCPSocket.hpp>

#include "Request.hpp"
#include "Response.hpp"
#include "apps/App.hpp"
#include "middlewares/Middleware.hpp"

using std::list;

/**
 * @brief Simple Webserver that parses a http request and generates a response
 */
class Webserver {

public:

    using Ptr = std::shared_ptr<Webserver>;

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
     * Adds a given app shared_ptr to m_apps and registers the routes of the app
     * @param app
     */
    void addApp(std::shared_ptr<App> app);

    /**
     * Adds a given middelware to m_middelwares to call processRequest() and processResponse()
     *  in the main loop
     * @param middleware
     */
    void addMiddleware(std::shared_ptr<Middleware> middleware);

    /**
     * @brief starts the main loop of the webserver
     */
    void serve();

private:

    socketwrapper::TCPSocket::Ptr m_socket; /// Underlying TCP Socket for communication

    list<std::shared_ptr<App>> m_apps; /// List with registered Apps

    list<std::shared_ptr<Middleware>> m_middelwares; /// List with registered Middelwares

};


#endif //CPPWEBSERVER_WEBSERVER_HPP
