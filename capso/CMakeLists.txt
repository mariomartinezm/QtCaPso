set(CMAKE_CXX_VISIBILITY_PRESET     hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

include(FetchContent)

FetchContent_Declare(pcg
    GIT_REPOSITORY https://github.com/imneme/pcg-cpp
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
)

FetchContent_GetProperties(pcg)
if(NOT pgc_POPULATED)
    FetchContent_Populate(pcg)
endif()

add_library(pcg INTERFACE)
target_include_directories(pcg INTERFACE ${pcg_SOURCE_DIR}/include)

# The preferred practice is not to specify the type of library to be built and
# leave the choice to the user
add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    src/cellularautomaton.cpp
    src/globalcapso.cpp
    src/localcapso.cpp
    src/randomnumber.cpp
    src/swarm.cpp)
target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
    include/cellularautomaton.h
    include/globalcapso.h
    include/localcapso.h)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

# Add the library's binary directory so other targets find the export file
target_include_directories(${PROJECT_NAME}
    PUBLIC include
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE src
    PRIVATE "$<BUILD_INTERFACE:${pcg_SOURCE_DIR}>/include")

target_link_libraries(${PROJECT_NAME} pcg)
