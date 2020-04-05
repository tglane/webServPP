//
// Created by timog on 02.03.19.
//

#include "TestApp.hpp"

void TestApp::register_routes()
{
    add_route("/normal", std::bind(&TestApp::show, this, std::placeholders::_1, std::placeholders::_2));
    add_route("/redirect", std::bind(&TestApp::showRedirect, this, std::placeholders::_1, std::placeholders::_2));
    add_route("/index", std::bind(&TestApp::showTemplate, this, std::placeholders::_1, std::placeholders::_2));
    add_route("/ajax", std::bind(&TestApp::ajaxTest, this, std::placeholders::_1, std::placeholders::_2));
}

void TestApp::show(Request& req, Response& res)
{
    res.set_body_from_file("/templates/test.html");
}

void TestApp::showRedirect(Request& req, Response& res)
{
    res.send_redirect("/test/normal");
}

void TestApp::showTemplate(Request& req, Response& res)
{
    std::list<std::string> l {"Hallo", "dies", "ist", "ein", "Test", "der", "Template-Enginge", "!"};

    res.set_body_from_template("test.tmpl.html",
                               std::map<std::string, std::variant<string, int, std::list<std::string>>> {{"headline", "Ja moin"},
                                                                                                        {"text",     "löl hier ist ja jz doch was xD"},
                                                                                                        {"its",      l},
                                                                                                        {"it",       4}});
}

void TestApp::ajaxTest(Request& req, Response& res)
{
    if(req.get_method() == "POST") {
        res.set_body(req.get_post_param("new_text"));
    }
}

