//
// Created by timog on 01.03.19.
//

#include "Statuscodes.hpp"

namespace webserv
{

std::map<int, const char*> Statuscodes::_codes = {
        /* 1xx - Information */
        {100, "Continue"},
        {101, "Switching Protocols"},
        {102, "Processing"},

        /* 2xx - successful Operation */
        {200, "OK"},
        {201, "Created"},
        {202, "Accepted"},
        {203, "Non-Authoritative Information"},
        {204, "No Content"},
        {205, "Reset Content"},
        {206, "Partial Content"},
        {207, "Multi-Status"},
        {208, "Already Reported"},
        {226, "IM Used"},

        /* 3xx - Redirect */
        {300, "Multiple Choices"},
        {301, "Moved Permanently"},
        {302, "Found"},
        {303, "See Other"},
        {304, "Not Modified"},
        {305, "Use Proxy"},
        {306, "(reserved)"},
        {307, "Temporary Request"},
        {308, "Permanent Redirect"},

        /* 4xx - Client Error */
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {402, "Payment Required"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {405, "Method Not Allowed"},
        {406, "Not Acceptable"},
        {407, "Proxy Authentication Required"},
        {408, "Request Time-Out"},
        {409, "Conflict"},
        {410, "Gone"},
        {411, "Length Required"},
        {412, "Precondition Failed"},
        {413, "Request Entity Too Large"},
        {414, "Request-URL Too Long"},
        {415, "Unsupported Media Type"},
        {416, "Requested range not satisfiable"},
        {417, "Expectation Failed"},
        {418, "I'm a teapot"},
        {420, "Policy Not Fullfilled"},
        {421, "There are too many connections from your internet address"},
        {422, "Unprocessable Entity"},
        {423, "Locked"},
        {424, "Failed Dependency"},
        {425, "Unordered Collection"},
        {426, "Upgrade Required"},
        {428, "Precondition Required"},
        {429, "Too Many Requests"},
        {431, "Request Header Fields Too Large"},

        /* 5xx - Server Error */
        {500, "Internal Server Error"},
        {501, "Not Implemented"},
        {502, "Bad Gateway"},
        {503, "Service Unavailable"},
        {504, "Gateway Time-Out"},
        {505, "HTTP Version not supported"},
        {506, "Variant Also Negotiates"},
        {507, "Insufficient Storage"},
        {508, "Loop Detected"},
        {509, "Bandwith Limit Exceeded"},
        {510, "Not Extended"}
};

std::map<const char*, const char*> Statuscodes::_mime_types {
        /* text mime types */
        {".js", "text/javascript"},
        {".css", "text/css"},
        {".html", "text/html"},
        {".htm", "text/html"},
        {".shtml", "text/html"},

        /* image mime types */
        {".ico", "image/x-icon"},
        {".png", "image/png"},
        {".jpg", "image/jpeg"},
        {".jepg", "image/jpeg"},
        {".jpe", "image/jpeg"},
        {".gif", "image/gif"},
        {".bmp", "image/bmp"}
};

const std::string Statuscodes::get_phrase(int code)
{
    const auto& it = _codes.find(code);
    if(it != _codes.end())
    {
        return (*it).second;
    }
    else
    {
        return "";
    };
}

const std::string Statuscodes::get_mime_type(const std::string& file_extension)
{
    const auto& it = _mime_types.find(file_extension.c_str());
    if(it != _mime_types.end())
    {
        return it->second;
    }
    else
    {
        return "";
    }
}

}

