//
// Created by timog on 16.04.19.
//

#include "RequestChecker.hpp"

//TODO add more checks
bool RequestChecker::checkRequest(Request &req)
{
    if(!checkRequestline(req.getMethod(), req.getResource(), req.getProtocol())) return false;

    return true;
}

//TODO improve requestline check
bool RequestChecker::checkRequestline(const string &method, const string &resource, const string &protocol)
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
    if(protocol.find("HTTP/") == string::npos && protocol.find("http/") == string::npos)
    {
        return false;
    }
    return true;
}