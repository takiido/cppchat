cmake_minimum_required(VERSION 3.30)
project(cppchat_api)

set(CMAKE_CXX_STANDARD 20)

add_library(cppchat_api STATIC)

target_sources(cppchat_api
        PRIVATE
            src/main.cpp
            src/utils.cpp
        PUBLIC
            include/group.h
            include/message.h
)

target_include_directories(cppchat_api
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external/nlohmann
)