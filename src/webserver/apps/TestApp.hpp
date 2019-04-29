//
// Created by timog on 02.03.19.
//

#ifndef CPPWEBSERVER_TESTAPP_HPP
#define CPPWEBSERVER_TESTAPP_HPP

#include "App.hpp"

class TestApp : public App {

    void registerRoutes() override;

    void show();

    void showRedirect();

    void showTemplate();

};


#endif //CPPWEBSERVER_TESTAPP_HPP
