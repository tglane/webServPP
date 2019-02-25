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

    while(1)
    {
        socketwrapper::TCPSocket::Ptr conn = m_socket->accept();

        Request::Ptr req = std::make_shared<Request>();
        req->parse(conn->readOnce());

        Response::Ptr res = std::make_shared<Response>(conn, req);
        res->send();
        conn->close();
    }

}