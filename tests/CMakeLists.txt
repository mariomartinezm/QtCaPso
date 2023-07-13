option(ENABLE_UNIT_TESTS "Enable units tests" ON)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

if(ENABLE_UNIT_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
    )

    # For windows: Prevent overriding the parent's compiler / linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()

set(TEST_SOURCES
    main.cpp
    randomnumber-test.cpp
    capso-test.cpp)

add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})

target_compile_options(${PROJECT_NAME}_test PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(${PROJECT_NAME}_test PUBLIC gtest_main capso)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
