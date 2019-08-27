//
// Created by timog on 02.03.19.
//

#include "App.hpp"

namespace webserv
{

void App::register_routes(webserv::CallbackHandler &handler)
{
    for(const auto& it : m_routes)
    {
        handler.add_route(it.first, it.second);
    }
}

}
