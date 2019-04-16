cmake_minimum_required(VERSION 3.5.1)

project(cppwebserver)
set(CMAKE_CXX_STANDARD 11)

set(SOURCES
        src/webserver/util/Statuscodes.cpp
        src/webserver/util/RequestChecker.cpp
        src/webserver/templating/Jinja2CppLight.cpp
        src/webserver/templating/stringhelper.cpp
        src/webserver/apps/App.cpp
        src/webserver/Webserver.cpp
        src/webserver/middlewares/LoggingMiddleware.cpp
        src/webserver/Request.cpp
        src/webserver/Response.cpp
        src/webserver/Cookie.cpp
        src/main.cpp

        src/webserver/apps/TestApp.cpp
   )

find_package(Threads)

find_library(SOCKETWRAPPER_LIB socketwrapper)
find_path(SOCKETWRAPPER_INC socketwrapper)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${SOCKETWRAPPER_LIB})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOCKETWRAPPER_INC})

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
