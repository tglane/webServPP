//
// Created by timog on 26.05.19.
//

#ifndef WEBSERV_PP_UUID4GENERATOR_HPP
#define WEBSERV_PP_UUID4GENERATOR_HPP

#include <sstream>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include <string>

using std::string;

class UUID4Generator {

public:

    static UUID4Generator instance() {
        static UUID4Generator generator;
        return generator;
    }

    string generate_uuid4() {
        string uuid;
        uuid += generate_hex(4);
        uuid += "-";
        uuid += generate_hex(2);
        uuid += "-";
        uuid += generate_hex(2);
        uuid += "-";
        uuid += generate_hex(4);
        return uuid;
    }

private:

    UUID4Generator() = default;

    static int random_int() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 255);
        return dis(gen);
    }

    static string generate_hex(const unsigned int len) {
        std::stringstream ss;
        for(auto i = 0; i < len; i++) {
            auto r = random_int();
            std::stringstream hexstream;
            hexstream << std::hex << r;
            auto hex = hexstream.str();
            ss << (hex.length() < 2 ? '0' + hex : hex);
        }
        return ss.str();
    }

};

#endif //WEBSERV_PP_UUID4GENERATOR_HPP
