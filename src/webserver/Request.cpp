//
// Created by timog on 19.02.19.
//

#include "Request.hpp"
#include <iostream>

//TODO secure parsing by checking for valid http requests
void Request::parse(char* request)
{
    m_request = string(request);
    std::cout << m_request << std::endl;
    //TODO: log the request using the logging middleware - to be implemented

    /* extract the request line */
    std::istringstream f(m_request);
    string request_line;
    getline(f, request_line);
    parse_requestline(request_line);

    /* Parse resource to path and params */
    size_t pos_q;
    if((pos_q = m_resource.find('?')) == string::npos)
    {
        m_path = m_resource;
    }
    else
    {
        m_path = m_resource.substr(0, pos_q);
        parse_params(m_resource.substr(pos_q + 1));
    }

    /* Read and parse request headers */
    string headerline;
    while(getline(f, headerline) && headerline != "\r")
    {
        size_t pos = headerline.find(':');
        if(pos != string::npos) {
            m_headers.insert(std::pair<string, string>(headerline.substr(0, pos), headerline.substr(pos+2)));
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
        it = m_headers.find("Content-Length");
        if(it != m_headers.end() && stoi(it->second) != 0)
        {
            char* body = new char[stoi(it->second)];
            f.read(body, stoi(it->second));
            parse_params(body);
            delete[] body;
        }
    }
}

void Request::parse_requestline(string& requestline)
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

void Request::parse_params(string param_string)
{
    //std::cout << param_string << std::endl;
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
            //std::cout << param << std::endl;
            size_t pos = param.find('=');
            std::pair<string, string> p(param.substr(0,pos), param.substr(pos + 1));
            //std::cout << p.first << " | " << p.second << std::endl;
            m_params.insert(p);
        }
    }
}

void Request::parse_cookies(string cookies)
{
    //std::cout << cookies << std::endl;
    std::istringstream iss(cookies);
    std::vector<string> cookies_split((std::istream_iterator<string>(iss)),
                                          std::istream_iterator<string>());

    for(auto it = cookies_split.begin(); it != cookies_split.end(); it++)
    {
        if((*it).find(";") != string::npos)
        {
            *it = (*it).substr(0, (*it).length() - 1);
        }
        size_t pos = (*it).find("=");
        Cookie c((*it).substr(0, pos), (*it).substr(pos + 1));
        m_cookies.insert(std::pair<string, Cookie>((*it).substr(0, pos), c));
    }
}
