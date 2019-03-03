cmake_minimum_required(VERSION 3.5.1)

project(cppwebserver)
set(CMAKE_CXX_STANDARD 11)

set(SOURCES
        src/Webserver/util/Statuscodes.cpp
        src/Webserver/apps/App.cpp
        src/Webserver/Webserver.cpp
        src/Webserver/Request.cpp
        src/Webserver/Response.cpp
        src/Webserver/Cookie.cpp
        src/main.cpp
   )

find_library(SOCKETWRAPPER_LIB socketwrapper)
find_path(SOCKETWRAPPER_INC socketwrapper)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${SOCKETWRAPPER_LIB})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOCKETWRAPPER_INC})
