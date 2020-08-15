// 
// Created by timog on 05.04.20.
//

#include "DirectApp.hpp"

namespace webserv
{

DirectApp::DirectApp(const std::function<void(const Request&, Response&)>& direct_callback)
    : m_direct_callback(direct_callback)
{
    this->register_routes();
}

bool DirectApp::operator()(const std::string& route, const Request& req, Response& res) const
{
    m_direct_callback(req, res);
    return true;
}

void DirectApp::register_routes()
{
    this->add_route("", m_direct_callback);
}

}
