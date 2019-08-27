//
// Created by timog on 02.03.19.
//

#ifndef CPPWEBSERVER_APP_HPP
#define CPPWEBSERVER_APP_HPP

#include <memory>
#include <string>
#include <map>
#include <functional>

#include "../Response.hpp"
#include "../Request.hpp"
#include "../CallbackHandler.hpp"

namespace webserv
{

class Webserver;

/**
 * Wraps a web application to be used by the webserver
 * Adds its routes to the server and gives callback methods for all routes
 */
class App {

public:

    /**
     * Registers all routes and its callback function
     * @param handler callback function handler
     */
    void register_routes(webserv::CallbackHandler& handler);

protected:

    /// Protected default constructor to avoid instances of App
    explicit App() = default;

    /**
     * Adds a given route and its callback method to m_routes
     * @param route string representing the http route
     * @param handler callback method for the route -> must be implemented by the app!
     */
    //void add_route(std::string route, const std::function<void(Request&, Response&)>& handler);

    std::map<std::string, std::function<void(Request&, Response&)>> m_routes; /// key = route path; value = handler function

};

}

#endif //CPPWEBSERVER_APP_HPP
