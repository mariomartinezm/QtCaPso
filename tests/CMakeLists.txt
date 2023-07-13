set(TEST_SOURCES
    main.cpp
    randomnumber-test.cpp
    capso-test.cpp)

add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})

target_compile_options(${PROJECT_NAME}_test PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(${PROJECT_NAME}_test PUBLIC gtest_main pcg capso)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
