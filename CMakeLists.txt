cmake_minimum_required(VERSION 3.30)
project(cppchat)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(api)
add_subdirectory(app)
add_subdirectory(server)