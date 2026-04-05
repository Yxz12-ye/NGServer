cmake_minimum_required(VERSION 3.10.0)
project(Night_Guard VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(CMAKE_USE_VCPKG "Use vcpkg package manager" ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/use_vcpkg.cmake)

file(GLOB_RECURSE SOURCES "src/*.cpp")

find_package(Boost REQUIRED COMPONENTS asio)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::asio)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)