//
// Created by timog on 01.03.19.
//

#ifndef CPPWEBSERVER_STATUSCODES_HPP
#define CPPWEBSERVER_STATUSCODES_HPP

#include <map>
#include <string>

using std::string;
using std::map;

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
    static string getPhrase(string code);

private:

    static map<int, string> c_codes; /// static map containing all http status phrases mapped to their code as key

};


#endif //CPPWEBSERVER_STATUSCODES_HPP
