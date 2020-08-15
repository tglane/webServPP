#ifndef DIRECT_APP_HPP
#define DIRECT_APP_HPP

#include "App.hpp"

namespace webserv
{

/**
 * Connects a direct callback to a single route
  Example: "host:port/route"
 */
class DirectApp : public App
{
public:
   
    DirectApp() = delete;
    DirectApp(DirectApp&) = delete;
    DirectApp& operator=(DirectApp&) = delete;
    DirectApp(DirectApp&&) = default;
    DirectApp& operator=(DirectApp&&) = default;

    DirectApp(const std::function<void(const Request&, Response&)>& direct_callback);

    bool operator()(const std::string& route, const Request& req, Response& res) const override;

    void register_routes() override;

private:
    
    std::function<void(const Request&, Response&)> m_direct_callback;

};

}

#endif

