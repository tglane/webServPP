//
// Created by timog on 19.02.19.
//

#include "Webserver.hpp"

#include <ctime>

namespace webserv {

Webserver::Webserver(int port, int queue_size, bool enable_https)
        : reqCheck(), m_secure_socket(AF_INET, "/etc/ssl/certs/cert.pem", "/etc/ssl/private/key.pem"), m_socket(AF_INET) {
    m_port = port;
    if (enable_https) {
        m_enable_https = true;
        m_secure_socket.bind("0.0.0.0", m_port);
        m_secure_socket.listen(queue_size);
    } else {
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
    app->register_routes();
    m_apps.push_back(std::move(app));
}

void Webserver::add_middleware(std::unique_ptr<Middleware> middleware)
{
    m_middlewares.push_back(std::move(middleware));
}

void Webserver::serve()
{
    if (m_enable_https)
    {
        std::cout << "Listening on https://localhost:" << m_port << " via encrypted connection" << std::endl;
    }
    else
    {
        std::cout << "Listening on http://localhost:" << m_port << std::endl;
    }

    while (1)
    {
        //TODO limit thread creation?
        //TODO allow use of http and https socket at the same time?

        /* Handle incoming requests within a new thread */
        try
        {
            if(m_enable_https)
            {
                auto connection_socket = m_secure_socket.accept();
                std::thread t = std::thread(&Webserver::handle_connection, this, std::move(connection_socket));
                t.detach();
            }
            else
            {
                auto connection_socket = m_socket.accept();
                std::thread t = std::thread(&Webserver::handle_connection, this, std::move(connection_socket));
                t.detach();
            }
        }
        catch(socketwrapper::SocketAcceptingException &ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }
}

void Webserver::handle_connection(std::unique_ptr<socketwrapper::TCPSocket> conn)
{
    time_t begin = clock(); //TODO remove when finished

    /* wait until data is available */
    try
    {
        while(conn->bytes_available() == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    catch(socketwrapper::ReadBytesAvailableException &e) { return; } //TODO ?

    std::cout << "---New Request---conn sock:" << conn->get_socket_descriptor() << std::endl;

    std::shared_ptr<Request> req = std::make_shared<Request>(conn->read_all().get());
    std::cout << req->create_string() << std::endl; //TODO remove
    std::shared_ptr<Response> res = std::make_shared<Response>(req);

    if(!reqCheck.check_request(*req))
    {
        //TODO implement this in logging middleware
        res->set_code("400");
        res->add_header("Connection", "close");
        res->create_string();
        Webserver::send_response(*conn, *res);
        std::cout << "400 --- Bad Request" << std::endl;
        conn->close();
        return;
    }

    /* Process request from all registered middlewares */
    for(auto &it : m_middlewares)
    {
        it->process_request(*req, *res);
    }

    /* Try to process the called route from a registered app, css file or javascript file */
    bool processed = false;
    std::string &&path = req->get_path();
    std::size_t pos = path.rfind('.');
    if(pos != string::npos)
    {
        try
        {
            res->add_header("Content-Type", Statuscodes::get_mime_type(path.substr(pos)));
            res->set_body_from_file(path);
            processed = true;
        }
        catch(std::invalid_argument &e)
        {
            processed = false;
        }
    }
    else
    {
        for(auto it = m_apps.begin(); it != m_apps.end(); it++)
        {
            if((*it)->get_callback(path, *req, *res))
            {
                processed = true;
            }
        }
    }

    if(!processed)
    {
        res->set_code("404");
        res->set_body("<!DOCTYPE html><html><head><title>404 - Not Found</title><body><h1>404 - Not Found</h1></body></html>\r\n");
    }
    else
    {
        /* If route was processed process response from registered middlewares */
        for(auto &it : m_middlewares)
        {
            it->process_response(*req, *res);
        }
    }

    //TODO add connection: keep-alive feature
    res->add_header("Connection", "close");
    Webserver::send_response(*conn, *res);
    conn->close();

    /* TODO remove when finished. Measure time for request handling */
    time_t end = clock();
    std::cout << "Response-time: " << difftime(end, begin) / CLOCKS_PER_SEC << "\n" << std::endl;

}

void Webserver::send_response(socketwrapper::TCPSocket &conn, Response &response)
{
    conn.write(response.create_string().c_str());
}

}
