//
// Created by timog on 19.02.19.
//

#include "Webserver.hpp"

#define ENABLE_TIME_MEASUREMENT
#ifdef ENABLE_TIME_MEASUREMENT
#include <ctime>
#endif

namespace webserv {

/**
 * "Shortcut" to send a Response object over a socket to the caller
 * @param conn TCPSocket reference representing the connection to the caller
 * @param res Response reference to the response to send to the caller to answer his response
 */
void send_response(socketwrapper::TCPSocket& conn, Response& res)
{
    std::string tmp = res.to_string();
    conn.write<char>(tmp.c_str(), tmp.size());
}

Webserver::Webserver(int port, int queue_size)
    : m_port(port), m_enable_https(false), m_socket(AF_INET), m_secure_socket(AF_INET, "", "")
{
    m_socket.bind("0.0.0.0", m_port);
    m_socket.listen(queue_size);

    m_secure_socket.close();
}

Webserver::Webserver(int port, int queue_size, const char* cert_path, const char* key_path)
    : m_port(port), m_enable_https(true), m_socket(AF_INET), m_secure_socket(AF_INET, cert_path, key_path)
{
    m_secure_socket.bind("0.0.0.0", m_port);
    m_secure_socket.listen(queue_size);

    m_socket.close();
}

Webserver::Webserver(Webserver&& other) noexcept
    : m_port(other.m_port), m_enable_https(other.m_enable_https), m_socket(std::move(other.m_socket)), 
        m_secure_socket(std::move(other.m_secure_socket))
{
    this->m_apps = std::move(other.m_apps);
    this->m_middlewares = std::move(other.m_middlewares);

    other.m_port = 0;
}

Webserver::~Webserver()
{
    m_socket.close();
    m_secure_socket.close();
}

Webserver& Webserver::operator=(Webserver&& other)
{
    this->m_port = other.m_port;
    this->m_enable_https = other.m_enable_https;
    this->m_socket = std::move(other.m_socket);
    this->m_secure_socket = std::move(other.m_secure_socket);
    this->m_apps = std::move(other.m_apps);
    this->m_middlewares = std::move(other.m_middlewares);

    other.m_port = 0;

    return *this;
}

void Webserver::add_app(const char* key, std::unique_ptr<App> app)
{
    app->register_routes();
    m_apps[key] = std::move(app);
}

void Webserver::add_app(const std::string& key, std::unique_ptr<App> app)
{
    app->register_routes();
    m_apps[key] = std::move(app);
}

void Webserver::add_app(const char* key, const std::function<void(const Request&, Response&)>& direct_call)
{
    m_apps[key] = std::make_unique<DirectApp>(direct_call);
}

void Webserver::add_app(const std::string& key, const std::function<void(const Request&, Response&)>& direct_call)
{
    m_apps[key] = std::make_unique<DirectApp>(direct_call);
}

void Webserver::add_middleware(std::unique_ptr<Middleware> middleware)
{
    m_middlewares.emplace_back(std::move(middleware));
}

void Webserver::serve() const
{
    if (m_enable_https)
    {
        std::cout << "Listening on https://localhost:" << m_port << " via encrypted connection" << '\n';
    }
    else
    {
        std::cout << "Listening on http://localhost:" << m_port << '\n';
    }

    while(true)
    {
        //TODO limit thread creation?
        //TODO allow use of http and https socket at the same time?

        /* Handle incoming requests within a new thread */
        try
        {
            if(m_enable_https)
            {
                auto connection_socket = m_secure_socket.accept();
                std::thread t(&Webserver::handle_connection, this, std::move(connection_socket));
                t.detach();
            }
            else
            {
                auto connection_socket = m_socket.accept();
                std::thread t(&Webserver::handle_connection, this, std::move(connection_socket));
                t.detach();
            }
        }
        catch(socketwrapper::SocketAcceptingException &ex)
        {
            std::cout << ex.what() << '\n';
        }
    }
}

void Webserver::handle_connection(std::unique_ptr<socketwrapper::TCPSocket> conn) const
{

#ifdef ENABLE_TIME_MEASUREMENT
    time_t begin = clock();
#endif

    /* Wait until data is available */
    try
    {
        while(conn->bytes_available() == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    catch(socketwrapper::ReadBytesAvailableException &e) { return; } //TODO ?

    // std::cout << "---New Request---conn sock:" << conn->get_socket_descriptor() << '\n';

    Request req;
    try
    {
        req.parse(conn->read_vector<char>(1024).data());
        // req.parse(conn->read_all_vector<char>().data());
    }
    catch(const std::invalid_argument& ia) 
    { 
        std::cout << "invalid_request" << '\n'; 
        return;
    }
    Response res(req);

    if(!check_request(req))
    {
        res.set_code(400);
        res.add_header("Connection", "close");
        res.to_string();
        send_response(*conn, res);
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
            res.add_header("Content-Type", get_mime_type(path.substr(pos)));
            res.set_body_from_file(path);
        }
        catch(std::invalid_argument &e)
        {
            res.set_code(404);
            res.set_body("<!DOCTYPE html><html><head><title>404 - Not Found</title><body><h1>404 - Not Found</h1></body></html>\r\n");
            return;
        }
    }
    else
    {
        /* Calling an app to handle the request */
        std::string app_name(path.data() + path.find_first_not_of('/'));
        std::string app_route;
        if(app_name.find('/') != std::string::npos)
        {
            app_route = std::string(app_name.data() + app_name.find_first_of('/'));
            app_name = std::string(app_name.data(), app_name.find('/'));
        }

        const auto& it = m_apps.find(app_name);
        if(it == m_apps.end() || !it->second->get_callback(app_route, req, res))
        {
            res.set_code(404);
            res.set_body("<!DOCTYPE html><html><head><title>404 - Not Found</title><body><h1>404 - Not Found</h1></body></html>\r\n");
        }
    }

    /* If route was processed process response from registered middlewares */
    for(const auto& it : m_middlewares)
        it->process_response(req, res);

    //TODO add connection: keep-alive feature
    res.add_header("Connection", "close");
    std::cout << res.to_string() << '\n';
    send_response(*conn, res);
    conn->close();

#ifdef ENABLE_TIME_MEASUREMENT
    time_t end = clock();
    std::cout << "Response-time: " << difftime(end, begin) / CLOCKS_PER_SEC << "\n" << '\n';
#endif

}

}
