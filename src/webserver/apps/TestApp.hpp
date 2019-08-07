//
// Created by timog on 02.03.19.
//

#ifndef CPPWEBSERVER_TESTAPP_HPP
#define CPPWEBSERVER_TESTAPP_HPP

#include "App.hpp"
#include "../authentication/AuthenticationService.hpp"

class TestApp : public App {

    void register_routes() override;

    void show(Request& req, Response& res);

    void showRedirect(Request& req, Response& res);

    void showTemplate(Request& req, Response& res);

    void ajaxTest(Request& req, Response& res);

};


#endif //CPPWEBSERVER_TESTAPP_HPP
