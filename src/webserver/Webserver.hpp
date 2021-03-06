//
// Created by timog on 19.02.19.
//

#ifndef CPPWEBSERVER_WEBSERVER_HPP
#define CPPWEBSERVER_WEBSERVER_HPP

#include <list>
#include <map>
#include <thread>
#include <vector>
#include <socketwrapper/TCPSocket.hpp>
#include <socketwrapper/SSLTCPSocket.hpp>

#include "Request.hpp"
#include "Response.hpp"
#include "util/Util.hpp"
#include "apps/App.hpp"
#include "apps/DirectApp.hpp"
#include "middlewares/Middleware.hpp"

namespace webserv {

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
    
    Webserver(int port, int queue_size, const char* cert_path, const char* key_path);

    /**
     * @brief Move contructor
     * @param other Webserver rvalue to be moved
     */
    Webserver(Webserver&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~Webserver();

    Webserver& operator=(Webserver&& other);

    /**
     * Adds a given app to m_apps and registers the routes of the app
     * @param key name of the app in http routes
     * @param app containign sub routes
     */
    void add_app(const char* key, std::unique_ptr<App> app);
    
    void add_app(const std::string& key, std::unique_ptr<App> app);

    /**
     * Adds a given lambda to handle a given route
     * @param key name of the app in http routes
     * @param direct_response function to handle requests to specified route
     */
    void add_app(const char* key, const std::function<void(const Request&, Response&)>& direct_call);

    void add_app(const std::string& key, const std::function<void(const Request&, Response&)>& direct_call);

    /**
     * Adds a given middelware to m_middelwares to call processRequest() and processResponse()
     *  in the main loop
     * @param middleware
     */
    void add_middleware(std::unique_ptr<Middleware> middleware);

    /**
     * @brief starts the main loop of the webserver to handle incoming requests
     */
    void serve() const;
    
private:

    /**
     * @brief handles a connection and returns an answer to the caller
     * @param conn socket connection to the client
     */
    void handle_connection(std::unique_ptr<socketwrapper::TCPSocket> conn) const;

    int m_port;
    bool m_enable_https = false;

    /* Sockets for accepting incoming requests */
    socketwrapper::TCPSocket m_socket;
    socketwrapper::SSLTCPSocket m_secure_socket;

    std::map<std::string , std::unique_ptr<App>> m_apps;

    std::list<std::unique_ptr<Middleware>> m_middlewares; /// List with registered Middelwares

};

}

#endif //CPPWEBSERVER_WEBSERVER_HPP
