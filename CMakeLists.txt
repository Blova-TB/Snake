cmake_minimum_required(VERSION 4.0)

project(snake)

find_package(Boost REQUIRED)

add_executable(snake src/main.cpp)

if(WIN32)
    target_compile_definitions(snake PRIVATE _WIN32_WINNT=0x0A00)
endif()

target_link_libraries(snake PRIVATE Boost::headers)
