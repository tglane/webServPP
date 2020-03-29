cmake_minimum_required(VERSION 3.5.1)

project(webServ-pp)
set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        src/webserver/util/Statuscodes.cpp
        src/webserver/util/RequestChecker.cpp
        src/webserver/templating/Jinja2CppLight.cpp
        src/webserver/templating/stringhelper.cpp
        src/webserver/apps/App.cpp
        src/webserver/Webserver.cpp
        src/webserver/middlewares/SessionMiddleware.cpp
        src/webserver/middlewares/LoggingMiddleware.cpp
        src/webserver/authentication/AuthenticationService.cpp
        src/webserver/Request.cpp
        src/webserver/Response.cpp
        src/webserver/Cookie.cpp
        src/webserver/Session.cpp
        src/main.cpp

        src/webserver/apps/TestApp.cpp
)

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    link_directories(${OPENSSL_LIBRARIES})
    include_directories(${OPENSSL_INCLUDE_DIR})
endif(OPENSSL_FOUND)

find_package(Threads)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libsocketwrapper_static.a)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})

