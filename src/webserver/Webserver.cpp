//
// Created by timog on 19.02.19.
//

#include "Webserver.hpp"

Webserver::Webserver(int port, int queue_size)
{
    m_socket = std::make_shared<socketwrapper::TCPSocket>(AF_INET);
    m_socket->bind(port);
    m_socket->listen(queue_size);
}

Webserver::~Webserver()
{
    m_socket->close();
}

void Webserver::addApp(std::shared_ptr<App> app)
{
    app->registerRoutes();
    m_apps.push_back(app);
}

void Webserver::addMiddleware(std::shared_ptr<Middleware> middleware)
{
    m_middelwares.push_back(middleware);
}

void Webserver::serve()
{
    std::cout << "Listening on http://localhost:8080\n" << std::endl;

    while(1)
    {
        //TODO set a special response if there are no registered apps

        socketwrapper::TCPSocket::Ptr conn = m_socket->accept();

        Request::Ptr req = std::make_shared<Request>();
        req->parse(conn->readOnce());
        Response::Ptr res = std::make_shared<Response>(conn, req);

        /* Process request from all registered middlewares */
        for(auto it = m_middelwares.begin(); it != m_middelwares.end(); it++)
        {
            (*it)->processRequest(req, res);
        }

        /* Try to process the called route from a registered app */
        bool processed = false;
        for(auto it = m_apps.begin(); it != m_apps.end(); it++)
        {
            if((*it)->getCallback(req->getPath(), req, res))
            {
                processed = true;
            }
        }

        if(!processed)
        {
            res->setCode("404");
            char notFoundBody[] = "<!DOCTYPE html><html><head><title>404 - Not Found</title><body><h1>404 - Not Found</h1></body></html>\r\n";
            res->setBody(notFoundBody);
        }
        else
        {
            /* If route was processed process response from registered middlewares */
            for(auto it = m_middelwares.begin(); it != m_middelwares.end(); it++)
            {
                (*it)->processResponse(res);
            }
        }

        res->send();
        conn->close();
    }

}
