set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Concurrent REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        util.cpp
        Models/cellularautomaton.cpp
        Models/localcapso.cpp
        Models/globalcapso.cpp
        Models/swarm.cpp
        Models/randomnumber.cpp
        Models/randomnumber.cpp
        View/caview.cpp
        Controller/controller.cpp
        Controller/controller.ui
        Controller/localsettingsdialog.cpp
        Controller/localsettingsdialog.ui
        Controller/globalsettingsdialog.cpp
        Controller/globalsettingsdialog.ui
        Controller/batchdialog.cpp
        Controller/batchdialog.ui
        Controller/batchitem.cpp
        Controller/controller.qrc
)

add_executable(${PROJECT_NAME} WIN32 ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE Controller
    PRIVATE Models
    PRIVATE View
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent)
