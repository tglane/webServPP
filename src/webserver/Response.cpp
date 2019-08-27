//
// Created by timog on 23.02.19.
//

#include <fstream>
#include <sstream>

#include "templating/Jinja2CppLight.h"

#include "Response.hpp"

namespace webserv
{

std::mutex Response::c_file_mutex;

string Response::create_string()
{
    string response {};

    /* Set some response fields if missing */
    if(m_code.empty())
    {
        m_code = "200";
    }
    if(m_headers.find("Content-Type") == m_headers.end())
    {
        m_headers.insert(std::pair<string, string>("Content-Type", "text/html; charset=UTF-8"));
    }
    std::time_t now(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

    /* Begin with response line */
    response.append("HTTP/1.1 " + m_code + " " + get_phrase(m_code) + "\r\n" + "Date: " + std::ctime(&now));

    /* Append all cookies to response */
    for(auto& it : m_cookies) //TODO auto& or auto?
    {
        response.append(it.second.build_header() + "\r\n");
    }

    /* Append all headers to response */
    for(auto& it : m_headers) //TODO auto& or auto
    {
        response.append(it.first + ": " + it.second + "\r\n");
    }

    /* Append body to response line */
    response.append("\r\n");
    if(!m_body.empty())
    {
        response.append(m_body);
    }

    return response;
}

void Response::set_body_from_template(const string& templateFile, const map<string, std::variant<string, int, list<string>>>& values)
{
    /* Open template file and read it into a string if found */
    c_file_mutex.lock();
    if(templateFile.rfind("..") != string::npos)
    {
        c_file_mutex.unlock();
        throw std::invalid_argument("Detected not allowed characters in path!\n");
    }
    //TODO use only the filename and search only in allowed folders for the filename
    std::ifstream ifs("../src/templates/" + templateFile);
    if(!ifs.good())
    {
        c_file_mutex.unlock();
        throw std::invalid_argument("Requested file not found.\n");
    }
    std::stringstream sstr;
    sstr << ifs.rdbuf();
    string htmlTemplate(sstr.str());
    c_file_mutex.unlock();

    /* Substitue template file placeholders with the given values */
    Jinja2CppLight::Template aTemplate {htmlTemplate};

    for(const auto& it : values)
    {
        /* Substitute a string */
        try {
            aTemplate.setValue(it.first, std::get<string>(it.second));
        } catch (const std::bad_variant_access&) {}
        /* Substitute an int */
        try {
            aTemplate.setValue(it.first, std::get<int>(it.second));
        } catch (const std::bad_variant_access&) {}
        /* Substitute a list of strings */
        try {
            Jinja2CppLight::TupleValue tmp;
            for(auto& ite : std::get<list<string>>(it.second))
            {
                tmp.addValue(ite);
            }
            aTemplate.setValue(it.first, tmp);
        } catch (const std::bad_variant_access&) {}
    }

    this->set_body(aTemplate.render());
}

void Response::set_body_from_file(const string &bodyFile)
{
    /* Open template file and read it into a string if found */
    c_file_mutex.lock();
    if(bodyFile.rfind("..") != string::npos)
    {
        c_file_mutex.unlock();
        throw std::invalid_argument("Path contains not allowed characters!\n");
    }
    //TODO use only the filename and search only in allowed folders for the filename
    std::ifstream ifs("../src" + bodyFile);
    if(!ifs.good())
    {
        c_file_mutex.unlock();
        throw std::invalid_argument("Requested file not found.\n");
    }
    std::stringstream sstr;
    sstr << ifs.rdbuf();
    string htmlBody(sstr.str());
    c_file_mutex.unlock();
    this->set_body(htmlBody);
}

void Response::set_body(const string& body)
{
    m_body = body;
    add_header("Content-Length", std::to_string(m_body.size()));
}

void Response::set_redirect(const string& url)
{
    this->add_header("Location", url);
    this->set_code("302");
}

void Response::add_header(const string& key, const string& value)
{
    auto it = m_headers.find(key);
    if(it != m_headers.end())
    {
        it->second = value;
    } else {
        m_headers.insert(std::pair<string, string>(key, value));
    }
}

void Response::add_cookie(Cookie cookie)
{
    auto it = m_cookies.find(cookie.get_name());
    if(it != m_cookies.end())
    {
        it->second = cookie;
    } else {
        m_cookies.insert(std::pair<string, Cookie>(cookie.get_name(), cookie));
    }
}

void Response::set_content_type(const string& contentType)
{
    const auto& it = m_headers.find("Content-Type");
    if(it != m_headers.end())
    {
        it->second = contentType;
    }
    else
    {
        m_headers.insert(std::pair<string, string>("Content-Type", contentType));
    }
}

string Response::get_phrase(const string& code)
{
    string codephrase = Statuscodes::get_phrase(code);
    if(codephrase.empty())
    {
        set_code("500");
        return "Unknown statuscode requested in code";
    }
    return codephrase;
}

}
