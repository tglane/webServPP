//
// Created by timog on 02.03.19.
//

#include "TestApp.hpp"

void TestApp::registerRoutes()
{

    addRoute("/test/normal", std::bind(&TestApp::show, this));
    addRoute("/test/redirect", std::bind(&TestApp::showRedirect, this));
    addRoute("/test", std::bind(&TestApp::showTemplate, this));
}

void TestApp::show()
{
    char body[] = "<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title></head><body><h1 id=\"headline\">Goodbye, world!</h1><form id=\"main_form\" method=\"post\"><textarea>Hallo</textarea id=\"id_text\"><input type=\"submit\"></form></body></html>\r\n";

    m_res->setBody(body);
}

void TestApp::showRedirect()
{
    m_res->sendRedirect("/test");
}

void TestApp::showTemplate()
{
    list<string> l{"Hallo", "dies", "ist", "ein", "Test", "der", "Template-Enginge", "!"};

    m_res->setBodyFromTemplate("test.tmpl.html", map<string, std::variant<string, int, list<string>>>{{"headline", "Ja moin"}, {"text", "löl hier ist ja jz doch was xD"}, {"its", l}, {"it", 4}});
}
