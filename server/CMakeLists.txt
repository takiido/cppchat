cmake_minimum_required(VERSION 3.30)
project(cppchat_server)

set(CMAKE_CXX_STANDARD 20)

add_executable(cppchat_server
        src/main.cpp
)

target_link_libraries(cppchat_server cppchat_api)
target_include_directories(cppchat_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../api/include)