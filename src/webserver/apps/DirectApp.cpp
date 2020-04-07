// 
// Created by timog on 05.04.20.
//

#include "DirectApp.hpp"

namespace webserv
{

DirectApp::DirectApp(const std::function<void(Request&, Response&)>& direct_callback)
    : m_direct_callback(direct_callback)
{
    this->register_routes();
}

void DirectApp::register_routes()
{
    this->add_route("/index", m_direct_callback);
}

}

