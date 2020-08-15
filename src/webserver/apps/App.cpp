//
// Created by timog on 02.03.19.
//

#include "App.hpp"

namespace webserv
{

bool App::operator()(const std::string& route, const Request& req, Response& res) const
{
    return this->get_callback(route, req, res);
}

void App::add_route(std::string_view route, const std::function<void(const Request& req, Response& res)>& handler)
{
    if(route.front() == '/')
        route = std::string_view(route.data() + 1, route.size() - 1);

    if(route.back() == '/')
        route = std::string_view(route.data(), route.size() - 1);

    m_routes[std::string(route)] = handler;
}

bool App::get_callback(const std::string& route, const Request& req, Response& res) const
{
    int start_mod{0}, end_mod{0};
    // if(route.size() == 0)
    //     return false;

    if(route.front() == '/')
        start_mod = end_mod = 1;
        // route = std::string_view(route.data() + 1, route.size() - 1);

    if(route.back() == '/')
        end_mod = 1;
        // route = std::string_view(route.data(), route.size() - 1);

    // std::string tmp = std::string(route.data() + start_mod, route.size() - end_mod);
    // const auto& it = m_routes.find(std::string(route.data() + start, route.size() - end));
    // std::cout << tmp << '\n';

    const auto& it = m_routes.find((start_mod == 0 && end_mod == 0) ? route : 
        route.substr(start_mod, route.size() - end_mod));
    if(it != m_routes.end())
    {
        (it->second)(req, res);
        return true;
    }
    else
    {
        return false;
    }
}

}
