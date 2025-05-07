cmake_minimum_required(VERSION 3.30)
project(cppchat_app)

set(CMAKE_CXX_STANDARD 20)

add_executable(cppchat_app
        src/main.cpp
)

target_link_libraries(cppchat_app cppchat_api)
target_include_directories(cppchat_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../api/include)