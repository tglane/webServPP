//
// Created by timog on 16.04.19.
//

#include "RequestChecker.hpp"

//TODO add more checks
bool RequestChecker::checkRequest(Request& req)
{
    if(!checkRequestline(req.get_method(), req.get_resource(), req.get_protocol())) { return false; }
    //if(!checkHost(req)) { return false; }

    return true;
}

//TODO improve requestline check
bool RequestChecker::checkRequestline(const string& method, const string& resource, const string& protocol)
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

bool RequestChecker::checkHost(Request &req)
{
    try {
        req.get_headers().at("Host");
        return true;
    } catch(std::out_of_range& e) {
        return false;
    }
}
