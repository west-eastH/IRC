cmake_minimum_required(VERSION 3.25)
project(irc)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(irc
        Channel.cpp
        Channel.hpp
        echoClient.cpp
        echoServer.cpp
        main.cpp
        Server.cpp
        Server.hpp
        socket.cpp)
