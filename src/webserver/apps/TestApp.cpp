//
// Created by timog on 02.03.19.
//

#include "TestApp.hpp"

void TestApp::register_routes()
{
    add_route("/test/normal", std::bind(&TestApp::show, this, std::placeholders::_1, std::placeholders::_2));
    add_route("/test/redirect", std::bind(&TestApp::showRedirect, this, std::placeholders::_1, std::placeholders::_2));
    add_route("/test", std::bind(&TestApp::showTemplate, this, std::placeholders::_1, std::placeholders::_2));
    add_route("/test/ajax", std::bind(&TestApp::ajaxTest, this, std::placeholders::_1, std::placeholders::_2));
}

void TestApp::show(webserv::Request& req, webserv::Response& res)
{
    res.set_body_from_file("/templates/test.html");
}

void TestApp::showRedirect(webserv::Request& req, webserv::Response& res)
{
    res.send_redirect("/test");
}

void TestApp::showTemplate(webserv::Request& req, webserv::Response& res)
{
    std::list<std::string> l {"Hallo", "dies", "ist", "ein", "Test", "der", "Template-Enginge", "!"};

    res.set_body_from_template("test.tmpl.html",
                               std::map<std::string, std::variant<string, int, std::list<std::string>>> {{"headline", "Ja moin"},
                                                                                                        {"text",     "löl hier ist ja jz doch was xD"},
                                                                                                        {"its",      l},
                                                                                                        {"it",       4}});
}

void TestApp::ajaxTest(webserv::Request& req, webserv::Response& res)
{
    if(req.get_method() == "POST") {
        res.set_body(req.get_post_param("new_text"));
    }
}
