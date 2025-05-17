cmake_minimum_required(VERSION 3.30)
project(cppchat_migrate)

set(CMAKE_CXX_STANDARD 20)

set(LIBCLANG_DIR ${CMAKE_SOURCE_DIR}/../external/libclang/libclang)

add_executable(cppchat_migrate
        main.cpp
)

target_include_directories(cppchat_migrate
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/../api/include
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${LIBCLANG_DIR}/include
)

target_link_directories(cppchat_migrate
        PUBLIC
          ${LIBCLANG_DIR}/lib
)

target_link_libraries(cppchat_migrate
          cppchat_api
          clang
)
