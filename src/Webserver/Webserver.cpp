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

void Webserver::serve()
{
    std::cout << "Listening on http://localhost:8080\n" << std::endl;

    char body[] = "<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title><body><h1>Goodbye, world!</h1><form id=\"main_form\" method=\"post\"><textarea>Hallo</textarea id=\"id_text\"><input type=\"submit\"></form></body></html>\r\n";


    while(1)
    {
        socketwrapper::TCPSocket::Ptr conn = m_socket->accept();

        Request::Ptr req = std::make_shared<Request>();
        req->parse(conn->readOnce());

        Response::Ptr res = std::make_shared<Response>(conn, req);

        std::map<string, Cookie> m = req->getCookies();
        for(auto it = m.begin(); it != m.end(); it++)
        {
            res->addCookie((*it).second);
        }
        res->setBody(body);
        res->send();
        conn->close();
    }

}