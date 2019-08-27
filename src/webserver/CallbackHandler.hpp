//
// Created by timog on 15.08.19.
//

#ifndef WEBSERV_PP_CALLBACKHANDLER_HPP
#define WEBSERV_PP_CALLBACKHANDLER_HPP

#include <map>
#include <string>
#include <functional>
#include "Request.hpp"
#include "Response.hpp"

namespace webserv
{

class CallbackHandler {

public:

    static CallbackHandler& get_instance()
    {
        static CallbackHandler callback_handler;
        return callback_handler;
    }

    /**
     * Adds a route and its callback function to m_routes
     * @param route string representation of route
     * @param callback_function
     */
    void add_route(std::string route, const std::function<void (Request&, Response&)>& callback_function)
    {
        if(route.front() != '/')
        {
            route = '/' + route;
        }
        if(route.back() == '/')
        {
            route = route.substr(0, route.size() - 1);
        }
        m_routes[route] = callback_function;
    }

    /**
     * Calls the corresponding callback function of a given route; returns true if route was found
     * @param route
     * @param req current request to answer
     * @param res response object of this request
     * @return true if route and function were found, false else
     */
    bool get_callback(const string& route, Request& req, Response& res)
    {
        std::map<std::string, std::function<void (Request&, Response&)>>::iterator it;
        if(route.back() == '/')
        {
            it = m_routes.find(route.substr(0, route.size() - 1));
        }
        else
        {
            it = m_routes.find(route);
        }

        //const auto& it = m_routes.find(route);
        if(it != m_routes.end())
        {
            (it->second)(req, res);
            return true;
        }
        else return false;
    }

private:

    CallbackHandler() = default;

    /// Stores all routes and the corresponding callback functions
    std::map<std::string, std::function<void (Request&, Response&)>> m_routes;

};

}

#endif //WEBSERV_PP_CALLBACKHANDLER_HPP
