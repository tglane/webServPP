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

    void addApp(App app);

    void addRoute(string route, void (*handler));

    /**
     * @brief starts the main loop of the webserver
     */
    void serve();

private:

    socketwrapper::TCPSocket::Ptr m_socket;

    list<App> m_apps; /// List with registered Apps

};


#endif //CPPWEBSERVER_WEBSERVER_HPP
