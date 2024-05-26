cmake_minimum_required(VERSION 3.5)

project(asio_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

add_executable(server main_server.cpp)
add_executable(client main_client.cpp)
