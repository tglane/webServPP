//
// Created by timog on 02.03.19.
//

#ifndef CPPWEBSERVER_APP_HPP
#define CPPWEBSERVER_APP_HPP

#include <memory>
#include <string_view>
#include <map>
#include <functional>

#include "../Response.hpp"
#include "../Request.hpp"

namespace webserv
{

class Webserver;

/**
 * Wraps a web application to be used by the webserver
 * Adds its routes to the server and gives callback methods for all routes
 */
class App {

public:
    
    ~App() = default;

    /**
     * Searches for the callback function mapped to route and executes it
     * @param route route to be called
     * @param req reference to the calling request
     * @param res reference to the response to be send to the caller
     */
    virtual bool operator()(const std::string& route, const Request& req, Response& res) const;

    /**
     * Needs to be implemented by all Apps
     * Call addRoute with all routes to add from this method
     */
    virtual void register_routes() = 0;

    /**
     * Searches in m_routes for the given route and executes the handler to the route
     * @param route
     * @return true if a route was found, false else
     */
    bool get_callback(const std::string& route, const Request& req, Response& res) const;

protected:

    /// Protected default constructor to avoid instances of App
    explicit App() = default;
    App(App&) = delete;
    App& operator=(App&) = delete;
    App(App&&) = default;
    App& operator=(App&&) = default;


    /**
     * Adds a given route and its callback method to m_routes
     * @param route string representing the http route
     * @param handler callback method for the route -> must be implemented by the app!
     */
    void add_route(std::string_view route, const std::function<void(const Request&, Response&)>& handler);

    /// key = route path; value = handler function
    std::map<std::string, std::function<void(const Request&, Response&)>> m_routes; 
};

}

#endif //CPPWEBSERVER_APP_HPP

