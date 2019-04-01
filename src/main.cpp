//
// Created by timog on 16.02.19.
//
#include <string>
#include <iostream>

#include "webserver/Webserver.hpp"
#include "webserver/apps/App.hpp"

/* Include middleware headers */
#include "webserver/middlewares/LoggingMiddleware.hpp"

/* Include app headers */
#include "webserver/apps/TestApp.hpp"
#include "webserver/middlewares/LoggingMiddleware.hpp"

int main(int argc, char** argv)
{
    Webserver w(8080, 5);

    /* Create apps and add it to the server using w.addApp(shared_ptr<app_name> name) */
    w.addApp(std::make_shared<TestApp>());

    /* Create middleware and add it to the server using w.addMiddleware(shared_ptr<middleware_name> name) */
    w.addMiddleware(std::make_shared<LoggingMiddleware>());

    w.serve();

}