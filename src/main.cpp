//
// Created by timog on 16.02.19.
//
#include <string>
#include <iostream>

#include "Webserver.hpp"

int main(int argc, char** argv)
{
    Webserver w(8080, 5);
    w.serve();
}