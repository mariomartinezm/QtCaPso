cmake_minimum_required(VERSION 3.1)

project(ergocli)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts
    GIT_TAG v3.1.1
)

FetchContent_GetProperties(cxxopts)

if(NOT cxxopts_POPULATED)
    FetchContent_Populate(cxxopts)
    add_subdirectory(${cxxopts_SOURCE_DIR} ${cxxopts_BINARY_DIR})
endif()

set(SOURCES
    main.cpp
    ../src/Models/cellularautomaton.cpp
    ../src/Models/localcapso.cpp
    ../src/Models/swarm.cpp
    ../src/Models/randomnumber.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME} PRIVATE ../src)
target_link_libraries(${PROJECT_NAME} PRIVATE pcg-cpp)
target_include_directories(${PROJECT_NAME} PRIVATE "$<BUILD_INTERFACE:${cxxopts_SOURCE_DIR}>/include")

if(LINUX)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()
