//
// Created by timog on 02.03.19.
//

#include "App.hpp"

namespace webserv
{

void App::add_route(std::string route, const std::function<void(Request& req, Response& res)>& handler)
{
    if(route.front() != '/')
    {
        route = '/' + route;
    }
    if(route.back() == '/')
    {
        route = route.substr(0, route.size() - 1);
    }
    m_routes[route] = handler;
}

bool App::get_callback(std::string_view route, Request& req, Response& res)
{
    if(route.size() == 0)
        return false;

    if(route.back() == '/')
    {
        route = std::string_view(route.data(), route.size() - 1);
    }

    const auto& it = m_routes.find(route.data());
    if(it != m_routes.end())
    {
        (it->second)(req, res);
        return true;
    }
    else 
        return false;
}

}

