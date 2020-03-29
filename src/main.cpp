//
// Created by timog on 16.02.19.
//

#include <string>
#include <iostream>

#include "webserver/Webserver.hpp"
#include "webserver/apps/App.hpp"
#include "webserver/middlewares/Middleware.hpp"

/* Include middleware headers */
#include "webserver/middlewares/LoggingMiddleware.hpp"
#include "webserver/middlewares/SessionMiddleware.hpp"

/* Include app headers */
#include "webserver/apps/TestApp.hpp"

int main(int argc, char** argv)
{
    bool enable_https = false;
    int port = 80;

    /* Get input parameters */
    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-https") == 0)
        {
            enable_https = true;
            port = 443;
        }
        if(strcmp(argv[i], "-port") == 0)
        {
            try {
                string arg = argv[i+1];
                size_t pos;
                port = std::stoi(arg, &pos);
                if(pos < arg.size()) 
                { 
                    std::cerr << "Trailing character after" << arg << std::endl; 
                }
            }
            catch(std::invalid_argument &ex) 
            { 
                std::cerr << "Invalid port number" << std::endl; 
            }
            catch(std::out_of_range &ex) 
            { 
                std::cerr << "Argument out of range" << std::endl; 
            }
        }
    }

    Webserver w(port, 5, enable_https);

    /* Create apps and add it to the server using w.add_app(unique_ptr<app_name>()) */
    w.add_app("test", std::make_unique<TestApp>());

    w.add_app("index", [](Request& req, Response& res) {
        res.set_body("Hello World!");        
    });

    /* Create middleware and add it to the server using w.add_middleware(shared_ptr<middleware_name> name) */
    //w.add_middleware(std::make_shared<LoggingMiddleware>());
    w.add_middleware(std::make_unique<SessionMiddleware>());

    w.serve();
}

