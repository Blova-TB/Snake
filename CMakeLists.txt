cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(snake)


# DEPENDANCE --------------------------------

find_package(Boost REQUIRED)

set(BUILD_SHARED_LIBS OFF)

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(SFML)


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
    target_compile_definitions(snake_cli PRIVATE _WIN32_WINNT=0x0A00 SFML_STATIC)
    target_compile_definitions(snake_serv PRIVATE _WIN32_WINNT=0x0A00)
endif()


target_link_libraries(snake_cli PRIVATE Boost::headers sfml-graphics)
target_link_libraries(snake_serv PRIVATE Boost::headers)
