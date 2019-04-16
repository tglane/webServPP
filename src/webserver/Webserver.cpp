//
// Created by timog on 19.02.19.
//

#include "Webserver.hpp"
#include <socketwrapper/SSLTCPSocket.hpp>

//TODO implement redirecting option from http to https?
//TODO catch SocketAcceptingException (for https)
Webserver::Webserver(int port, int queue_size, bool enable_https)
    : reqCheck()
{
    m_port = port;
    if(enable_https)
    {
        m_enable_https = true;
        m_secure_socket = std::make_shared<socketwrapper::SSLTCPSocket>(AF_INET, "/etc/ssl/certs/cert.pem", "/etc/ssl/private/key.pem");
        m_secure_socket->bind("0.0.0.0", m_port);
        m_secure_socket->listen();
    }
    else
    {
        m_socket = std::make_shared<socketwrapper::TCPSocket>(AF_INET);
        m_socket->bind("0.0.0.0", m_port);
        m_socket->listen(queue_size);
    }
}

Webserver::~Webserver()
{
    m_socket->close();
}

void Webserver::addApp(const std::shared_ptr<App> app)
{
    app->registerRoutes();
    m_apps.push_back(app);
}

void Webserver::addMiddleware(const std::shared_ptr<Middleware> middleware)
{
    m_middlewares.push_back(middleware);
}

void Webserver::serve()
{
    if(m_enable_https)
    {
        std::cout << "Listening on https://localhost:" << m_port << " via encrypted https connection" << std::endl;
    }
    else
    {
        std::cout << "Listening on http://localhost:" << m_port << std::endl;
    }

    while(1)
    {
        //TODO set a special response if there are no registered apps
        //TODO limit thread creation?
        //TODO allow use of http and https socket at the same time?

        /* Handle incoming requests within a new thread */
        std::shared_ptr<std::thread> t;
        try {
            if (m_enable_https) {
                t = std::make_shared<std::thread>(&Webserver::handleConnection, this, m_secure_socket->accept());

            } else {
                t = std::make_shared<std::thread>(&Webserver::handleConnection, this, m_socket->accept());
            }

            t->detach();
            m_threads.push_back(t);
        } catch(socketwrapper::SocketAcceptingException &ex) {
            std::cout << "Client connection not accepted" << std::endl;
        }
    }
}

//TODO replace socket->read(bytes) with socket->readAll()
void Webserver::handleConnection(std::shared_ptr<socketwrapper::TCPSocket> conn)
{
    /* wait until data is available */
    while(conn->bytes_available() == 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    Request::Ptr req = std::make_shared<Request>();
    req->parse(conn->read(1024));

    if(!reqCheck.checkRequest(*req))
    {
        std::cout << "Incoming Request not valid --- Connection refused" << std::endl;
        return;
    }

    Response::Ptr res = std::make_shared<Response>(conn, req);

    /* Process request from all registered middlewares */
    for(auto it = m_middlewares.begin(); it != m_middlewares.end(); it++)
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
        for(auto it = m_middlewares.begin(); it != m_middlewares.end(); it++)
        {
            (*it)->processResponse(res);
        }
    }

    res->send();
    conn->close();
}
