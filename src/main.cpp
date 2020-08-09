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

void add_routes(webserv::Webserver& w)
{
    /* Create apps and add it to the server using w.add_app("route name", std::unique_ptr<"app_name">()) */
    w.add_app("test", std::make_unique<TestApp>());

    w.add_app("lol", [](webserv::Request& req, webserv::Response& res) {
        res.set_body("Hello World! This is an direct response.");
    });

    /* Create middleware and add it to the webserver using add_middleware(std::make_unique<"middleware_name">() */
    w.add_middleware(std::make_unique<webserv::SessionMiddleware>());
}

int main(int argc, char** argv)
{
    bool enable_https = false;
    int port = 80, queue_size = 5;

    /* Get input parameters */
    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "--https") == 0 || strcmp(argv[i], "-h") == 0)
        {
            enable_https = true;
            port = 443;
        }
        if(strcmp(argv[i], "--port") == 0 || strcmp(argv[i], "-p") == 0)
        {
            try {
                std::string arg(argv[i+1]);
                size_t pos;
                port = std::stoi(arg, &pos);
                if(pos < arg.size()) 
                    std::cerr << "Trailing character after " << arg << std::endl; 
            }
            catch(std::invalid_argument& ex) 
            { 
                std::cerr << "Invalid port number" << std::endl; 
            }
            catch(std::out_of_range& ex) 
            { 
                std::cerr << "Argument out of range" << std::endl; 
            }
        }
        if(strcmp(argv[i], "--queue") == 0 || strcmp(argv[i], "-q") == 0)
        {
            try {
                std::string arg(argv[i + 1]);
                size_t pos;
                queue_size = std::stoi(arg, &pos);
                if(pos < arg.size())
                    std::cerr << "Trailing character after " << arg << std::endl;
            }
            catch(std::invalid_argument& ex)
            {
                std::cerr << "Invalid queue size" << std::endl;
            }
            catch(std::out_of_range& ex)
            {
                std::cerr << "Argument out of range" << std::endl;
            }
        }
    }

    if(enable_https)
    {
        webserv::Webserver w(port, queue_size, "/etc/ssl/certs/cert.pem", "etc/ssl/private/key.pem");
        add_routes(w);
        w.serve();
    }
    else
    {
        webserv::Webserver w(port, queue_size);
        add_routes(w);
        w.serve();
    }

}

