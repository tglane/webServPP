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
    //std::cout << "Listening on http://localhost:8080" << std::endl;
    char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title><body><h1>Goodbye, world!</h1><form id=\"main_form\" method=\"post\"><textarea>Hallo</textarea id=\"id_text\"><input type=\"submit\"></form></body></html>\r\n";

    while(1)
    {
        socketwrapper::TCPSocket::Ptr conn = m_socket->accept();

        Request::Ptr r = std::make_shared<Request>();
        r->parse(conn->readOnce());

        conn->writeOnce(response);
        conn->close();
    }

}