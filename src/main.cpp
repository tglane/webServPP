//
// Created by timog on 16.02.19.
//
#include "../ext/socketwrapper/src/TCPSocket.hpp"
#include <string>
#include <iostream>

int main(int argc, char** argv)
{
    socketwrapper::TCPSocket::Ptr t(new socketwrapper::TCPSocket(AF_INET));
    t->bind(8080);
    t->listen(1);

    char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title><body><h1>Goodbye, world!</h1></body></html>\r\n";

    std::cout << "Listening on http://localhost:8080" << std::endl;

    while(1) {
        socketwrapper::TCPSocket::Ptr conn = t->accept();
        std::cout << "Connection received" << std::endl;

        conn->write(response);
    }
}