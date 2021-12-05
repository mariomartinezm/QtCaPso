set(TEST_SOURCES
    main.cpp
    ../src/Models/randomnumber.cpp
    randomnumber-test.cpp)

add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})

target_compile_options(${PROJECT_NAME}_test PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME}_test PRIVATE ../src)
target_link_libraries(${PROJECT_NAME}_test PUBLIC gtest_main pcg-cpp)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
