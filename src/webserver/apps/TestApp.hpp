//
// Created by timog on 02.03.19.
//

#ifndef CPPWEBSERVER_TESTAPP_HPP
#define CPPWEBSERVER_TESTAPP_HPP

#include "App.hpp"
#include "../authentication/AuthenticationService.hpp"

class TestApp : public webserv::App {

public:

    TestApp();
    //void register_routes(webserv::CallbackHandler& handler) override;

    void show(webserv::Request& req, webserv::Response& res);

    void showRedirect(webserv::Request& req, webserv::Response& res);

    void showTemplate(webserv::Request& req, webserv::Response& res);

    void ajaxTest(webserv::Request& req, webserv::Response& res);

};


#endif //CPPWEBSERVER_TESTAPP_HPP
