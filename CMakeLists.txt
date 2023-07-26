cmake_minimum_required(VERSION 3.2)

project(capso LANGUAGES CXX VERSION 0.3.2 DESCRIPTION "CaPso model library")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

add_subdirectory(capso)
add_subdirectory(CaPsoGui)
add_subdirectory(tests EXCLUDE_FROM_ALL)
