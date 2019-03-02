//
// Created by timog on 02.03.19.
//

#ifndef CPPWEBSERVER_APP_HPP
#define CPPWEBSERVER_APP_HPP

#include <string>


using std::string;

class App {

public:

    App() = default;

    void addRoute(string route, void (*handler));

private:



};

#endif //CPPWEBSERVER_APP_HPP
