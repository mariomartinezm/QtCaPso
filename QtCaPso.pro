RESOURCES += \
    controller.qrc

HEADERS += \
    swarm.h \
    particle.h \
    localsettingsdialog.h \
    localcapso.h \
    latticepoint.h \
    globalsettingsdialog.h \
    globalcapso.h \
    controller.h \
    cellularautomaton.h \
    caview.h \
    catype.h \
    util.h

FORMS += \
    localsettingsdialog.ui \
    globalsettingsdialog.ui \
    controller.ui

SOURCES += \
    swarm.cpp \
    particle.cpp \
    main.cpp \
    localsettingsdialog.cpp \
    localcapso.cpp \
    latticepoint.cpp \
    globalsettingsdialog.cpp \
    globalcapso.cpp \
    controller.cpp \
    cellularautomaton.cpp \
    caview.cpp \
    util.cpp

# This option should only be used in linux or mingw
!win32{
    QMAKE_CXXFLAGS += -std=c++11
}
