//
// Created by timog on 02.03.19.
//

#include "App.hpp"

void App::addRoute(string route, std::function<void()> handler)
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

bool App::getCallback(string route, Request::Ptr req, Response::Ptr res)
{
    if(route.back() == '/')
    {
        route = route.substr(0, route.size() - 1);
    }

    auto it = m_routes.find(route);
    if(it != m_routes.end())
    {
        m_req = std::move(req);
        m_res = std::move(res);
        (it->second)();
        return true;
    }
    else return false;
}
