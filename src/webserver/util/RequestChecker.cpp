//
// Created by timog on 16.04.19.
//

#include "RequestChecker.hpp"

namespace webserv
{

//TODO improve requestline check
static bool check_requestline(const std::string& method, const std::string& resource, const std::string& protocol)
{
    if(method != "post" && method != "POST" && method != "get" && method != "GET" &&
       method != "head" && method != "HEAD" && method != "put" && method != "PUT" &&
       method != "delete" && method != "DELETE" && method != "trace" && method != "TRACE" &&
       method != "patch" && method != "PATCH")
    {
        return false;
    }
    if(resource.find("..") != string::npos)
    {
        return false;
    }
    if(!std::regex_match(protocol, std::regex("(HTTP/[0-9].[0-9])|(http/[0-9][.][0-9])")))
    {
        return false;
    }
    return true;
}

static bool check_host(const Request &req)
{
    try {
        req.get_headers().at("Host");
        return true;
    } catch(std::out_of_range& e) {
        return false;
    }
}

//TODO add more checks
bool check_request(const Request& req)
{
    if(!check_requestline(req.get_method(), req.get_resource(), req.get_protocol())) 
        return false;
    //if(!check_host(req)) { return false; }

    return true;
}

}
