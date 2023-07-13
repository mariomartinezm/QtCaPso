project(CaPsoGui LANGUAGES CXX VERSION 0.3.2 DESCRIPTION "GUI for the CaPso model")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Concurrent REQUIRED)

add_executable(${PROJECT_NAME} WIN32)
target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
    util.h
    util.cpp
    View/caview.h
    View/caview.cpp
    Controller/controller.h
    Controller/controller.cpp
    Controller/controller.ui
    Controller/localsettingsdialog.h
    Controller/localsettingsdialog.cpp
    Controller/localsettingsdialog.ui
    Controller/globalsettingsdialog.h
    Controller/globalsettingsdialog.cpp
    Controller/globalsettingsdialog.ui
    Controller/batchdialog.h
    Controller/batchdialog.cpp
    Controller/batchdialog.ui
    Controller/batchdialog.h
    Controller/batchitem.cpp
    Controller/controller.qrc)

target_include_directories(${PROJECT_NAME}
    PRIVATE Controller
    PRIVATE Models
    PRIVATE View
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent)
target_link_libraries(${PROJECT_NAME} PRIVATE capso)
