include(Models/Models.pri)
include(View/View.pri)
include(Controller/Controller.pri)

HEADERS += \
    util.h \

SOURCES += \
    main.cpp \
    util.cpp \

# This option should only be used in linux or mingw
!win32 | win32-g++{
    QMAKE_CXXFLAGS += -std=c++11
}
