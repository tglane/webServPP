//
// Created by timog on 19.02.19.
//

#include "Webserver.hpp"

Webserver::Webserver(int port, int queue_size, bool enable_https)
    : reqCheck(), m_secure_socket(AF_INET, "/etc/ssl/certs/cert.pem", "/etc/ssl/private/key.pem"), m_socket(AF_INET)
{
    m_port = port;
    if(enable_https)
    {
        m_enable_https = true;
        m_secure_socket.bind("0.0.0.0", m_port);
        m_secure_socket.listen(queue_size);
    }
    else
    {
        m_socket.bind("0.0.0.0", m_port);
        m_socket.listen(queue_size);
    }
}

Webserver::~Webserver()
{
    m_socket.close();
}

void Webserver::add_app(std::unique_ptr<App> app)
{
    app->registerRoutes();
    m_apps.push_back(std::move(app));
}

void Webserver::add_middleware(std::unique_ptr<Middleware> middleware)
{
    m_middlewares.push_back(std::move(middleware));
}

void Webserver::serve()
{
    if(m_enable_https)
    {
        std::cout << "Listening on https://localhost:" << m_port << " via encrypted connection" << std::endl;
    }
    else
    {
        std::cout << "Listening on http://localhost:" << m_port << std::endl;
    }

    while(1)
    {
        //TODO limit thread creation?
        //TODO allow use of http and https socket at the same time?

        /* Handle incoming requests within a new thread */
        std::thread t;
        try {
            if (m_enable_https) {
                this->handle_connection(std::move(m_secure_socket.accept()));

                /*t = std::thread(&Webserver::handle_connection, this, m_secure_socket.accept().get());
                t.detach();*/
            } else {
                this->handle_connection(std::move(m_socket.accept()));

                /*t = std::thread(&Webserver::handle_connection, this, m_socket.accept().get());
                t.detach();*/
            }
        } catch(socketwrapper::SocketAcceptingException& ex) {
            std::cout << ex.what() << std::endl;
        }
    }
}

void Webserver::handle_connection(std::unique_ptr<socketwrapper::TCPSocket> conn)
{
    /* wait until data is available */
    try
    {
        while(conn->bytes_available() == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    catch(socketwrapper::ReadBytesAvailableException& e) { return; } //TODO ?

    std::cout << "---New Request---" << std::endl;

    std::shared_ptr<Request> req = std::make_shared<Request>();
    auto buffer = conn->read_all();
    req->parse(buffer.get());
    std::shared_ptr<Response> res = std::make_shared<Response>(req);

    if(!reqCheck.checkRequest(*req))
    {
        //TODO implement this in logging middleware
        res->set_code("400");
        res->add_header("Connection", "close");
        res->create_string();
        std::cout << "400 --- Bad Request" << std::endl;
        conn->close();
        return;
    }

    /* Process request from all registered middlewares */
    for(auto& it : m_middlewares)
    {
        it->processRequest(*req, *res);
    }

    /* Try to process the called route from a registered app, css file or javascript file */
    bool processed = false;
    string&& path = req->get_path();

    if(std::regex_match(path, std::regex("(.*\\.(js|css|ico|img|jpg|png))")))
    {
        try
        {
            res->set_body_from_file(path);
            processed = true;
        }
        catch(std::invalid_argument& e)
        {
            processed = false;
            std::cout << e.what() << std::endl;
        }
    }

    for(auto it = m_apps.begin(); it != m_apps.end() && !processed; it++)
    {
        if((*it)->getCallback(path, *req, *res))
        {
            processed = true;
        }
    }

    if(!processed)
    {
        res->set_code("404");
        char notFoundBody[] = "<!DOCTYPE html><html><head><title>404 - Not Found</title><body><h1>404 - Not Found</h1></body></html>\r\n";
        res->set_body(notFoundBody);
    }
    else
    {
        /* If route was processed process response from registered middlewares */
        for(auto& it : m_middlewares)
        {
            it->processResponse(*res);
        }
    }

    bool sent = false;
    try {
        string connection_type = req->get_headers().at("Connection");
        if(connection_type.find("keep-alive") != string::npos)
        {
            res->add_header("Connection", "keep-alive");
            this->send_response(*conn, *res);
            this->handle_connection(std::move(conn));
            sent = true;
        }
    } catch(std::out_of_range& e)
    {
        res->add_header("Connection", "close");
        this->send_response(*conn, *res);
        conn->close();
    }
}

void Webserver::send_response(socketwrapper::TCPSocket& conn, Response& response)
{
    conn.write(response.create_string().c_str());
}
