set(TEST_BIN ${PROJECT_NAME}_test)

set(TEST_SOURCES
    main.cpp
    ../src/Models/randomnumber.cpp
    randomnumber-test.cpp)

add_executable(${TEST_BIN} ${TEST_SOURCES})

add_test(NAME  {${TEST_BIN} COMMAND ${TEST_BIN})

target_link_libraries(${TEST_BIN} PUBLIC gtest)
