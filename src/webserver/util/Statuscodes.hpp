//
// Created by timog on 01.03.19.
//

#ifndef CPPWEBSERVER_STATUSCODES_HPP
#define CPPWEBSERVER_STATUSCODES_HPP

#include <map>
#include <string>

namespace webserv
{

/**
 * @brief Container class for http status phrases
 */
class Statuscodes {

public:

    /**
     * @brief Gets the http status phrase to a given status code
     * @param code http status code
     * @return string representation of status phrase
     */
    const static std::string get_phrase(int code);

    /**
     * @brief Gets the mime type for a given file extension
     * @param file_extension
     * @return string representation of mime type
     */
    const static std::string get_mime_type(const std::string& file_extension);

private:

    static std::map<int, const char*> _codes;

    static std::map<const char*, const char*> _mime_types; //TODO complete list of mime types

};

}

#endif //CPPWEBSERVER_STATUSCODES_HPP
