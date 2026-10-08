cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(snake)

find_package(Boost REQUIRED)


# CLIENT -----------------------------------

file(GLOB_RECURSE CLIENT_SOURCES CONFIGURE_DEPENDS
    "src/client/*.cpp"
    "src/client/*.h"
    "src/shared/*.cpp"
    "src/shared/*.h"
)
add_executable(snake_cli ${CLIENT_SOURCES})


# SERVER -----------------------------------

file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS
    "src/server/*.cpp"
    "src/server/*.h"
    "src/shared/*.cpp"
    "src/shared/*.h"
)
add_executable(snake_serv ${SERVER_SOURCES})





if(WIN32)
    target_compile_definitions(snake_cli PRIVATE _WIN32_WINNT=0x0A00)
    target_compile_definitions(snake_serv PRIVATE _WIN32_WINNT=0x0A00)
endif()


target_link_libraries(snake_cli PRIVATE Boost::headers)
target_link_libraries(snake_serv PRIVATE Boost::headers)
