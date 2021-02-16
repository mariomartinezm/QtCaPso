cmake_minimum_required(VERSION 3.5)

project(QtCaPso LANGUAGES CXX)

add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(thirdparty/googletest)
    add_subdirectory(test)
endif()
