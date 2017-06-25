cmake_minimum_required(VERSION 3.6)
project(my_server)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -levent -levent_pthreads -lpthread")

set(SOURCE_FILES main.cpp HttpServer.cpp HttpServer.hpp Constants.cpp Constants.hpp HttpResponse.cpp HttpResponse.hpp HttpRequest.cpp HttpRequest.hpp)

add_executable(my_server ${SOURCE_FILES})