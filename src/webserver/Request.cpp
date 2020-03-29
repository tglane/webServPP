//
// Created by timog on 19.02.19.
//

#include "Request.hpp"

void Request::parse(const char* request)
{
    m_request = request;

    /* extract the request line */
    std::istringstream request_stringstream(m_request);
    std::string request_line;
    getline(request_stringstream, request_line);
    this->parse_requestline(request_line);

    /* Parse resource to path and params */
    size_t pos_q;
    if((pos_q = m_resource.find('?')) == std::string::npos)
    {
        m_path = m_resource;
    }
    else
    {
        m_path = std::string_view(m_resource.data(), pos_q);
        Request::parse_params(std::string_view(m_resource.data() + pos_q + 1), m_query_params);
    }

    /* Read and parse request headers */
    std::string headerline;
    while(getline(request_stringstream, headerline) && headerline != "\r")
    {
        size_t pos = headerline.find(':');
        if(pos != std::string::npos)
            m_headers.insert({ std::string_view(headerline.data(), pos), std::string_view(headerline.data() + pos + 2) });
    }

    /* Read cookies and put it in m_cookies */
    auto it = m_headers.find("Cookie");
    if(it != m_headers.end())
    {
        this->parse_cookies(it->second.data());
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
    std::string request;
    request.reserve(1024);
    request.append(m_method);
    request.append(" ");
    request.append(m_path);
    request.append("?");
    for(const auto& it : m_query_params)
    {
        request.append(it.first);
        request.append("=");
        request.append(it.second);
        request.append("&");
    }
    request.pop_back();
    //TODO append fragment to headerline (#...)
    request.append(" HTTP/1.1\r\n");

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

Cookie Request::get_cookie(const std::string& cookie_name)
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

void Request::parse_requestline(const std::string_view requestline)
{
    int i, last_index = 0, vec_index = 0;
    std::string_view tmp_store[3];

    for(i = 0; i < requestline.size() && vec_index < 3; i++)
    {
        if(requestline[i] == ' ' || i + 1 == requestline.size())
        {
            tmp_store[vec_index] = std::string_view(requestline.data(), i);
            last_index = i + 1;
            vec_index++;
        }
    }

    if(vec_index != 3)
        throw std::invalid_argument("invalid_requestline");

    m_method = tmp_store[0];
    m_resource = tmp_store[1];
    m_protocol = tmp_store[2];
}

void Request::parse_params(std::string_view param_string, std::map<std::string_view, std::string_view>& param_container)
{
    int offset = 0;
    for(int i = 0; i <= param_string.length(); i++)
    {
        if(param_string[i] == '&' || param_string[i] == '#' || i == param_string.length())
        {
            std::string_view param(param_string.data() + offset, i - offset);
            size_t pos = param_string.find('=');
            offset = i + 1;
            if(pos != std::string::npos)
            {
                param_container.insert({ std::string_view(param.data(), pos), std::string_view(param.data() + pos + 1) });
            }
        }
    }
}

void Request::parse_cookies(const std::string& cookies)
{
    //std::cout << cookies << std::endl;
    std::istringstream iss(cookies);
    std::vector<std::string> cookies_split((std::istream_iterator<std::string>(iss)),
                                          std::istream_iterator<std::string>());

    for(auto& it : cookies_split)
    {
        if(it.find(';') != string::npos)
        {
            it = it.substr(0, it.length() - 1);
        }
        size_t pos = it.find('=');
        Cookie c(it.substr(0, pos), it.substr(pos + 1));
        m_cookies.insert(std::pair<std::string, Cookie>(it.substr(0, pos), c));
    }

}

std::string Request::get_from_map(const std::map<std::string_view, std::string_view>& container, const std::string& key)
{
    try
    {
        return std::string(container.at(key));
    }
    catch(std::out_of_range& e)
    {
        return "";
    }
}

