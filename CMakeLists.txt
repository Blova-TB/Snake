cmake_minimum_required(VERSION 4.0)

project(snake)

find_package(Boost REQUIRED)

add_executable(snake_cli src/client/main.cpp)
add_executable(snake_serv src/server/main.cpp)



if(WIN32)
    target_compile_definitions(snake_cli PRIVATE _WIN32_WINNT=0x0A00)
    target_compile_definitions(snake_serv PRIVATE _WIN32_WINNT=0x0A00)
endif()



target_link_libraries(snake_cli PRIVATE Boost::headers)
target_link_libraries(snake_serv PRIVATE Boost::headers)
