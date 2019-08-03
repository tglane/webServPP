//
// Created by timog on 01.03.19.
//

#ifndef CPPWEBSERVER_STATUSCODES_HPP
#define CPPWEBSERVER_STATUSCODES_HPP

#include <map>
#include <string>

/**
 * @brief Container class for http status phrases
 */
class Statuscodes {

public:

    /**
     * @brief Gets the http status phrase to a given status code
     * @param code
     * @return string representation of status phrase
     */
    const static std::string get_phrase(const std::string& code);

    /**
     * @brief Gets the mime type for a given file extension
     * @param file_extension
     * @return string representation of mime type
     */
    const static std::string get_mime_type(const std::string& file_extension);

private:

    static std::map<int, std::string> _codes;

    static std::map<std::string, std::string> _mime_types; //TODO complete list of mime types

};


#endif //CPPWEBSERVER_STATUSCODES_HPP
