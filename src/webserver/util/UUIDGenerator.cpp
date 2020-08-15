//
// Created by timog on 26.05.19.
//

#include "Util.hpp"

#include <sstream>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>

namespace webserv
{

std::string generate_uuid4() {
    auto random_int = []()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 255);
        return dis(gen);
    };

    auto generate_hex = [&random_int](const unsigned int len)
    {
        std::stringstream ss;
        for(auto i = 0; i < len; i++)
        {
            auto r = random_int();
            std::stringstream hexstream;
            hexstream << std::hex << r;
            auto hex = hexstream.str();
            ss << (hex.length() < 2 ? '0' + hex : hex);
        }

        return ss.str();
    };

    std::string uuid;
    uuid += generate_hex(4);
    uuid += "-";
    uuid += generate_hex(2);
    uuid += "-";
    uuid += generate_hex(2);
    uuid += "-";
    uuid += generate_hex(4);
    return uuid;
}

// static int random_int() {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<int> dis(0, 255);
//     return dis(gen);
// }

// static std::string generate_hex(const unsigned int len) {
//     std::stringstream ss;
//     for(auto i = 0; i < len; i++) {
//         auto r = random_int();
//         std::stringstream hexstream;
//         hexstream << std::hex << r;
//         auto hex = hexstream.str();
//         ss << (hex.length() < 2 ? '0' + hex : hex);
//     }
//     return ss.str();
// }

}
