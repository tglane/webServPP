//
// Created by timog on 02.03.19.
//

#include "TestApp.hpp"

void TestApp::registerRoutes()
{

    addRoute("/test/normal", std::bind(&TestApp::show, this));
    addRoute("/test/redirect", std::bind(&TestApp::showRedirect, this));
    addRoute("/test", std::bind(&TestApp::showTemplate, this));
    addRoute("/test/ajax", std::bind(&TestApp::ajaxTest, this));
}

void TestApp::show()
{
    m_res->setBodyFromFile("/templates/test.html");
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

void TestApp::ajaxTest()
{
    if(m_req->getMethod() == "POST") {
        m_res->setBody("<div id=\"messages\"><div>" + m_req->getParams().at("new_text") + "</div><br>" + m_req->getParams().at("old_text") + "</div>");
    }
}
