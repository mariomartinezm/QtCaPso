cmake_minimum_required(VERSION 3.1)

project(ergocli)

set(SOURCES
    main.cpp
    ../src/Models/cellularautomaton.cpp
    ../src/Models/localcapso.cpp
    ../src/Models/swarm.cpp
    ../src/Models/randomnumber.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME} PRIVATE ../src)
target_link_libraries(${PROJECT_NAME} PUBLIC pcg-cpp)
