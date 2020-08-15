//
// Created by timog on 02.03.19.
//

#ifndef CPPWEBSERVER_TESTAPP_HPP
#define CPPWEBSERVER_TESTAPP_HPP

#include "App.hpp"
#include "../authentication/AuthenticationService.hpp"

class TestApp : public webserv::App {

    void register_routes() override;

    void show(const webserv::Request& req, webserv::Response& res);

    void showRedirect(const webserv::Request& req, webserv::Response& res);

    void showTemplate(const webserv::Request& req, webserv::Response& res);

    void ajaxTest(const webserv::Request& req, webserv::Response& res);

};

#endif //CPPWEBSERVER_TESTAPP_HPP

