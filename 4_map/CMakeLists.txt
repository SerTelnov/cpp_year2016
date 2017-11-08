project(map)
cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra -Wno-char-subscripts -Wno-unused-result -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -O2 -g")

add_definitions(-D_GLIBCXX_DEBUG)

file(GLOB_RECURSE SOURCE_FILES source/*.cpp tests/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.h tests/*.h)

include_directories(include)
include_directories(2_promise_future/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "4_map")
set(3D_PARTY_HOME ../../3dparty)

set(ENV{GTEST} y)

if ("$ENV{GTEST}" STREQUAL "y")
    message("!Gtests're enabled!")

    include_directories(${3D_PARTY_HOME}/googletest-1.8.0)
    link_directories(${3D_PARTY_HOME}/googletest-1.8.0)

    include_directories(tests)
    add_definitions(-D_GTEST)
endif ()

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

if ("$ENV{GTEST}" STREQUAL "y")
    target_link_libraries(${PROJECT_NAME} googletest)
endif ()

target_link_libraries(${PROJECT_NAME} pthread)
