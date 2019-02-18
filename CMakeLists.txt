cmake_minimum_required(VERSION 3.5.1)

project(cppwebserver)
set(CMAKE_CXX_STANDARD 11)

set(SOURCES
        ext/socketwrapper/src/BaseSocket.cpp
        ext/socketwrapper/src/UDPSocket.cpp
        ext/socketwrapper/src/TCPSocket.cpp
        ext/socketwrapper/Exceptions/SocketCreationException.cpp
        ext/socketwrapper/Exceptions/SocketCloseException.cpp
        ext/socketwrapper/Exceptions/SocketBoundException.cpp
        ext/socketwrapper/Exceptions/SocketBindException.cpp
        src/main.cpp
   )

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC socketwrapper)
