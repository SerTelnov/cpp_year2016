set(CMAKE_CXX_COMPILER "clang++")

project(promise_future)
cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pedantic -Wall -Wextra -Wno-char-subscripts -Wno-unused-result -g -fsanitize=thread")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -O2 -g")

#add_definitions(-D_LINUX)
#add_definitions(-DNDEBUG)
add_definitions(-D_GLIBCXX_DEBUG)

file(GLOB_RECURSE SOURCE_FILES source/*.cpp tests/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.h tests/*.h)

include_directories(include)

if ("$ENV{GTEST}" STREQUAL "y")
    message("!Gtests're enabled!")

    find_package(GTest REQUIRED)

    include_directories(tests)
    add_definitions(-D_GTEST)
endif ()

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

if ("$ENV{GTEST}" STREQUAL "y")
    target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES} gmock)
endif ()

target_link_libraries(${PROJECT_NAME} pthread)
