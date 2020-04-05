#ifndef DIRECT_APP_HPP
#define DIRECT_APP_HPP

#include "App.hpp"

class DirectApp : public App
{
public:
   
    DirectApp() = delete;
    DirectApp(DirectApp&) = delete;
    DirectApp& operator=(DirectApp&) = delete;
    DirectApp(DirectApp&&) = default;
    DirectApp& operator=(DirectApp&&) = default;

    DirectApp(const std::function<void(Request&, Response&)>& direct_callback);

    void register_routes() override;

private:
    
    std::function<void(Request&, Response&)> m_direct_callback;

};

#endif

