//
// Created by timog on 19.02.19.
//

#include "Request.hpp"

namespace webserv
{

void Request::parse(const char* request)
{
    m_request = std::string(request);

    /* extract the request line */
    std::istringstream request_stringstream {m_request};
    std::string request_line;
    getline(request_stringstream, request_line);
    this->parse_requestline(request_line);

    /* Parse resource to path and params */
    size_t pos_q;
    if((pos_q = m_resource.find('?')) == string::npos)
    {
        m_path = m_resource;
    }
    else
    {
        m_path = m_resource.substr(0, pos_q);
        std::string param_string {m_resource.substr(pos_q + 1)};
        Request::parse_params(m_resource.substr(pos_q + 1), m_query_params);
    }

    /* Read and parse request headers */
    std::string headerline;
    while(getline(request_stringstream, headerline) && headerline != "\r")
    {
        size_t pos = headerline.find(':');
        if(pos != string::npos) {
            m_headers.insert(std::pair<std::string, std::string>(headerline.substr(0, pos), headerline.substr(pos+2)));
        }
    }

    /* Read cookies and put it in m_cookies */
    auto it = m_headers.find("Cookie");
    if(it != m_headers.end())
    {
        parse_cookies(it->second);
    }

    /* Parse POST parameters */
    if(m_method == "POST" || m_method == "post")
    {
        try
        {
            int length = stoi(this->get_header("Content-Length"));
            if(length > 0) {
                char body[length];
                request_stringstream.read(body, length);
                Request::parse_params(body, m_body_params);
            }
        }
        catch(std::invalid_argument& e)
        {
            /* Exception occurs if header "Content-Length" is 0.
               No exception handling needed */
        }
    }
}

std::string Request::create_string()
{
    std::string request {m_method + " "};
    (request += m_path) += "?";
    for(const auto& it : m_query_params)
    {
        (((request += it.first) += "=") += it.second) += "&";
    }
    request.pop_back();
    //TODO append fragment to headerline (#...)
    request += " HTTP/1.1\r\n";

    for(const auto& it : m_headers)
    {
        (((request += it.first) += ": ") += it.second) += "\r\n";
    }
    request += "\r\n";
    for(const auto& it : m_body_params)
    {
        (((request += it.first) += "=") += it.second) += "&";
    }
    request.pop_back();

    return request;
}

Cookie Request::get_cookie(const std::string &cookie_name)
{
    try
    {
        return m_cookies.at(cookie_name);
    }
    catch(std::out_of_range& e)
    {
        throw std::out_of_range("No matching cookie found");
    }
}

void Request::parse_requestline(const string& requestline)
{
    std::istringstream iss(requestline);
    std::vector<string> requestline_split((std::istream_iterator<string>(iss)),
                                            std::istream_iterator<string>());

    if((requestline_split.capacity() - 1) == 3)
    {
        m_method = requestline_split[0];
        m_resource = requestline_split[1];
        m_protocol = requestline_split[2];
    }
    else
    {
        return; //TODO: Add Exception here
    }
}

void Request::parse_params(string&& param_string, std::map<std::string, std::string>& param_container)
{
    if(param_string.find('#') == string::npos)
    {
        param_string.append("&");
    }

    int tmp = 0;
    for(int i = 0; i < param_string.length(); i++)
    {
        if(param_string[i] == '&' || param_string[i] == '#')
        {
            string param = param_string.substr((size_t) tmp, (size_t) (i-tmp));
            tmp = i + 1;
            size_t pos = param.find('=');
            std::pair<string, string> p(param.substr(0,pos), param.substr(pos + 1));
            param_container.insert(p);
        }
    }
}

void Request::parse_cookies(const string& cookies)
{
    std::istringstream iss(cookies);
    std::vector<string> cookies_split((std::istream_iterator<string>(iss)),
                                          std::istream_iterator<string>());

    for(auto& it : cookies_split)
    {
        if(it.find(';') != string::npos)
        {
            it = it.substr(0, it.length() - 1);
        }
        size_t pos = it.find('=');
        Cookie c(it.substr(0, pos), it.substr(pos + 1));
        m_cookies.insert(std::pair<string, Cookie>(it.substr(0, pos), c));
    }
}

std::string Request::get_from_map(const std::map<std::string, std::string>& container, const std::string &key)
{
    try
    {
        return container.at(key);
    }
    catch(std::out_of_range& e)
    {
        return "";
    }
}

}
