cmake_minimum_required(VERSION 3.30)
project(cppchat_api)

set(CMAKE_CXX_STANDARD 20)

add_library(cppchat_api STATIC
        src/main.cpp
)

target_include_directories(cppchat_api PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)