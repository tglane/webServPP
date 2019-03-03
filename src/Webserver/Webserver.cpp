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

void Webserver::addMiddelware()
{
    //TODO
}

void Webserver::serve()
{
    std::cout << "Listening on http://localhost:8080\n" << std::endl;

    char body[] = "<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title><body><h1>Goodbye, world!</h1><form id=\"main_form\" method=\"post\"><textarea>Hallo</textarea id=\"id_text\"><input type=\"submit\"></form></body></html>\r\n";

    while(1)
    {
        //TODO set a special response if there are no registered apps

        socketwrapper::TCPSocket::Ptr conn = m_socket->accept();

        Request::Ptr req = std::make_shared<Request>();
        req->parse(conn->readOnce());
        Response::Ptr res = std::make_shared<Response>(conn, req);

        //TODO call processRequesest from middelwares

        bool processed = false;
        for(auto it = m_apps.begin(); it != m_apps.end(); it++)
        {
            if((*it)->getCallback(req->getPath(), req, res) == true)
            {
                processed = true;
            }
        }

        //TODO call processRespond from middelwares

        if(!processed)
        {
            //TODO send response with code 404 not found and small html site with message
        }
        else res->send();

        conn->close();
    }

}
