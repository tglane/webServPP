//
// Created by timog on 16.02.19.
//
#include <string>
#include <iostream>

#include "Webserver/Webserver.hpp"
#include "Webserver/apps/App.hpp"

/* Include app headers */

int main(int argc, char** argv)
{
    /* Create apps and add it to the server using w.addApp(app_name) */
    Webserver w(8080, 5);
    w.serve();

}