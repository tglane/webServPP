//
// Created by timog on 02.03.19.
//

#include "App.hpp"
#include <iostream>
App::App()
{
    this->add_route("index", [](Request&, Response&) {});
}

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
    m_routes.emplace(route, handler);
}

bool App::get_callback(std::string_view route, Request& req, Response& res)
{
    if(route.size() == 0)
        return false;

    std::cout << "route " << route << std::endl;

    if(route.back() == '/')
    {
        route = std::string_view(route.data(), route.size() - 1);
    }

    for(const auto& tmp : m_routes)
        std::cout << "[DEBUG}" << tmp.first << std::endl;

    const auto& it = m_routes.find(route.data());
    if(it != m_routes.end())
    {
        std::cout << "good" << std::endl;
        (it->second)(req, res);
        return true;
    }
    else 
        std::cout << "bad" << std::endl;
        return false;
}

