//
// Created by timog on 19.02.19.
//

#include "Webserver.hpp"

#include <ctime>

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

    std::cout << m_socket.get_socket_descriptor() << std::endl;
}

Webserver::~Webserver()
{
    m_socket.close();
}

void Webserver::add_app(const char* key, std::unique_ptr<App> app)
{
    app->register_routes();
    m_apps[std::string(key)] = std::move(app);
}

void Webserver::add_app(const std::string& key, std::unique_ptr<App> app)
{
    app->register_routes();
    m_apps[key] = std::move(app);
}

void Webserver::add_app(const char* key, const std::function<void(Request&, Response&)>& direct_call)
{
    // TODO
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
        try {
            if (m_enable_https)
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
        catch(socketwrapper::SocketAcceptingException& ex)
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
        while(conn->bytes_available() == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    catch(socketwrapper::ReadBytesAvailableException& e) { return; } //TODO ?

    std::cout << "---New Request---conn sock:" << conn->get_socket_descriptor() << std::endl;

    // Request req(reinterpret_cast<char*>(conn->read_vector(1024).data()));
    Request req;
    try
    {
        std::vector<char> tmp = conn->read_all_vector();
        std::cout << "[DEBUG] " << tmp.data() << std::endl;
        req.parse(tmp.data());
        // req.parse(conn->read_all_vector().data());
    }
    catch(const std::invalid_argument& ia) { std::cout << "invalid_request" << std::endl; return; }

    // std::cout << req.create_string() << std::endl; // TODO remove
    Response res(req);

    if(!reqCheck.check_request(req))
    {
        res.set_code(400);
        res.add_header("Connection", "close");
        res.create_string();
        Webserver::send_response(*conn, res);
        std::cout << "400 --- Bad Request" << std::endl;
        conn->close();
        return;
    }

    /* Process request from all registered middlewares */
    for(const auto& it : m_middlewares)
        it->process_request(req, res);

    /* Try to process the called route from a registered app, css file or javascript file */
    std::string&& path = req.get_path();
    size_t pos = path.rfind('.');
    if(pos != string::npos)
    {
        /* Send a requested file to the client without invoking an app */
        // TODO improve way of sending files to the client
        try
        {
            res.add_header("Content-Type", Statuscodes::get_mime_type(path.substr(pos)));
            res.set_body_from_file(path);
        }
        catch(std::invalid_argument& e)
        {
            Webserver::send_error(*conn, res, 404);
            return;
        }
    }
    else
    {
        /* Calling an app to handle the request */
        std::string_view app_name(path.data() + path.find_first_not_of('/'));
        std::string_view app_route;
        if(app_name.find('/') != std::string::npos)
        {
            app_route = std::string_view(app_name.data() + app_name.find_first_of('/'));
            app_name = std::string_view(app_name.data(), app_name.find('/'));
        }

        const auto& it = m_apps.find(std::string(app_name).data());
        if(it == m_apps.end() || 
            !it->second->get_callback((app_route.size() == 0 || app_route == "/") ? "/index" : app_route, req, res))
        {
            // TODO try to get index app if its defined
            Webserver::send_error(*conn, res, 404);
            return;
        }
    }

    /* If route was processed process response from registered middlewares */
    for(const auto& it : m_middlewares)
        it->process_response(req, res);

    //TODO add connection: keep-alive feature
    res.add_header("Connection", "close");
    Webserver::send_response(*conn, res);
    conn->close();

    /* TODO remove when finished. Measure time for request handling */
    time_t end = clock();
    std::cout << "Response-time: " << difftime(end, begin) / CLOCKS_PER_SEC << "\n" << std::endl;

}

void Webserver::send_response(socketwrapper::TCPSocket& conn, Response& res)
{
    conn.write(res.create_string().c_str());
}

void Webserver::send_error(socketwrapper::TCPSocket& conn, Response& res, int code)
{
    res.set_code(404);
    res.set_body("<!DOCTYPE html><html><head><title>404 - Not Found</title><body><h1>404 - Not Found</h1></body></html>\r\n");
    conn.write(res.create_string().c_str());
    conn.close();
}

