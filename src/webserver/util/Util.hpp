#ifndef WEBSERV_PP_UTIL_H
#define WEBSERV_PP_UTIL_H

#include "../Request.hpp"

#include <string>

namespace webserv {

/**
 * @brief Generates a pseudo-random UUID4 represented as a std string
 * @return string representation of a UUID4 number
 */
std::string generate_uuid4();

/**
 * @brief Checks wether a given request object represents a valid http request
 * @param req reference to a request object to be checked
 */
bool check_request(const Request& req);

/**
 * @brief Gets the http status phrase to a given status code
 * @param code http status code
 * @return string representation of status phrase
 */
std::string get_statuscode_phrase(int code);

/**
 * @brief Gets the mime type for a given file extension
 * @param file_extension
 * @return string representation of mime type
 */
std::string get_mime_type(const std::string& file_extension);

}

#endif
