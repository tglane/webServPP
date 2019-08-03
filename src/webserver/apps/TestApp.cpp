//
// Created by timog on 02.03.19.
//

#include "TestApp.hpp"

void TestApp::registerRoutes()
{
    addRoute("/test/normal", std::bind(&TestApp::show, this, std::placeholders::_1, std::placeholders::_2));
    addRoute("/test/redirect", std::bind(&TestApp::showRedirect, this, std::placeholders::_1, std::placeholders::_2));
    addRoute("/test", std::bind(&TestApp::showTemplate, this, std::placeholders::_1, std::placeholders::_2));
    addRoute("/test/ajax", std::bind(&TestApp::ajaxTest, this, std::placeholders::_1, std::placeholders::_2));
}

void TestApp::show(Request& req, Response& res)
{
    res.set_body_from_file("/templates/test.html");
}

void TestApp::showRedirect(Request& req, Response& res)
{
    res.send_redirect("/test");
}

void TestApp::showTemplate(Request& req, Response& res)
{
    list<string> l{"Hallo", "dies", "ist", "ein", "Test", "der", "Template-Enginge", "!"};

    res.set_body_from_template("test.tmpl.html",
                               map<string, std::variant<string, int, list<string>>> {{"headline", "Ja moin"},
                                                                                    {"text",     "löl hier ist ja jz doch was xD"},
                                                                                    {"its",      l},
                                                                                    {"it",       4}});
}

void TestApp::ajaxTest(Request& req, Response& res)
{
    if(req.get_method() == "POST") {
        res.set_body("<div id=\"messages\"><div>" + req.get_params().at("new_text") + "</div><br>" +
                     req.get_params().at("old_text") + "</div>");
    }
}
