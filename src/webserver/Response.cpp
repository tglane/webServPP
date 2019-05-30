//
// Created by timog on 23.02.19.
//

#include <fstream>
#include <sstream>

#include "templating/Jinja2CppLight.h"

#include "Response.hpp"

std::mutex Response::mu;

string Response::createString()
{
    string response;

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
    response.append("HTTP/1.1 " + m_code + " " + getPhrase(m_code) + "\r\n" + "Date: " + std::ctime(&now));

    /* Append all cookies to response */
    for(auto it = m_cookies.begin(); it != m_cookies.end(); it++)
    {
        response.append((*it).second.buildHeader() + "\r\n");
    }

    /* Append all headers to response */
    for(auto it = m_headers.begin(); it != m_headers.end(); it++)
    {
        response.append(it->first + ": " + it->second + "\r\n");
    }

    /* Append body to response line */
    response.append("\r\n");
    if(!m_body.empty())
    {
        response.append(m_body);
    }

    return response;
}

void Response::setBodyFromTemplate(const string& templateFile, map<string, std::variant<string, int, list<string>>> values)
{
    /* Open template file and read it into a string if found */
    mu.lock();
    if(templateFile.rfind("..") != string::npos)
    {
        mu.unlock();
        throw std::invalid_argument("Detected not allowed characters in path!\n");
    }
    //TODO use only the filename and search only in allowed folders for the filename
    std::ifstream ifs("../src/templates/" + templateFile);
    if(!ifs.good())
    {
        mu.unlock();
        throw std::invalid_argument("Requested file not found.\n");
    }
    std::stringstream sstr;
    sstr << ifs.rdbuf();
    string htmlTemplate(sstr.str());
    mu.unlock();

    /* Substitue template file placeholders with the given values */
    Jinja2CppLight::Template aTemplate(htmlTemplate);

    for(auto it = values.begin(); it != values.end(); it++)
    {
        /* Substitute a string */
        try {
            aTemplate.setValue(it->first, std::get<string>(it->second));
        } catch (const std::bad_variant_access&) {}
        /* Substitute an int */
        try {
            aTemplate.setValue(it->first, std::get<int>(it->second));
        } catch (const std::bad_variant_access&) {}
        /* Substitute a list of strings */
        try {
            Jinja2CppLight::TupleValue tmp;
            for(auto ite = std::get<list<string>>(it->second).begin(); ite != std::get<list<string>>(it->second).end(); ite++)
            {
                tmp.addValue(*ite);
            }
            aTemplate.setValue(it->first, tmp);
        } catch (const std::bad_variant_access&) {}
    }

    setBody(aTemplate.render());
}

void Response::setBodyFromFile(const string &bodyFile)
{
    /* Open template file and read it into a string if found */
    mu.lock();
    if(bodyFile.rfind("..") != string::npos)
    {
        mu.unlock();
        throw std::invalid_argument("Path contains not allowed characters!\n");
    }
    //TODO use only the filename and search only in allowed folders for the filename
    std::ifstream ifs("../src" + bodyFile);
    if(!ifs.good())
    {
        mu.unlock();
        throw std::invalid_argument("Requested file not found.\n");
    }
    std::stringstream sstr;
    sstr << ifs.rdbuf();
    string htmlBody(sstr.str());
    mu.unlock();
    setBody(htmlBody);
}

void Response::setBody(const string& body)
{
    m_body = body;
    addHeader("Content-Length", std::to_string(m_body.size()));
}

void Response::sendRedirect(const string& url)
{
    addHeader("Location", url);
    setCode("302");
    createString();
}

void Response::addHeader(const string& key, const string& value)
{
    auto it = m_headers.find(key);
    if(it != m_headers.end())
    {
        it->second = value;
    } else {
        m_headers.insert(std::pair<string, string>(key, value));
    }
}

void Response::addCookie(Cookie cookie)
{
    auto it = m_cookies.find(cookie.getName());
    if(it != m_cookies.end())
    {
        it->second = cookie;
    } else {
        m_cookies.insert(std::pair<string, Cookie>(cookie.getName(), cookie));
    }
}

void Response::setContentType(const string& contentType)
{
    auto it = m_headers.find("Content-Type");
    if(it != m_headers.end())
    {
        it->second = contentType;
    }
    else
    {
        m_headers.insert(std::pair<string, string>("Content-Type", contentType));
    }
}

string Response::getPhrase(const string& code)
{
    string codephrase = Statuscodes::getPhrase(code);
    if(codephrase.empty())
    {
        setCode("500");
        return "Unknown statuscode requested in code";
    }
    return codephrase;
}
