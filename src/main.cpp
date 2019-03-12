//
// Created by timog on 16.02.19.
//
#include <string>
#include <iostream>

#include "Webserver/Webserver.hpp"
#include "Webserver/apps/App.hpp"

/* Include app headers */
#include "Webserver/apps/TestApp.hpp"

int main(int argc, char** argv)
{
    Webserver w(8080, 5);

    /* Create apps and add it to the server using w.addApp(shared_ptr<app_name> name) */

    /* Create middlewares and add it to the server using w.addMiddleware(shared_ptr<middleware_name> name) */

    w.serve();

}