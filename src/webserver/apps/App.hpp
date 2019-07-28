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

using std::map;
using std::string;

class Webserver;

/**
 * Wraps a web application to be used by the webserver
 * Adds its routes to the server and gives callback methods for all routes
 */
class App {

public:

    /// Constructor
    explicit App() = default;

    /**
     * Needs to be implemented by all Apps
     * Call addRoute with all routes to add from this method
     */
    virtual void registerRoutes() = 0;

    /**
     * Searches in m_routes for the given route and executes the handler to the route
     * @param route
     * @return true if a route was found, false else
     */
    bool getCallback(string route, Request& req, Response& res);

protected:

    /**
     * Adds a given route and its callback method to m_routes
     * @param route string representing the http route
     * @param handler callback method for the route -> must be implemented by the app!
     */
    void addRoute(string route, std::function<void(Request&, Response&)>);

    map<string, std::function<void(Request&, Response&)>> m_routes; /// key = route path; value = handler function

};

#endif //CPPWEBSERVER_APP_HPP
