INCLUDEPATH += $$PWD \

HEADERS += \
    Models/cellularautomaton.h \
    Models/localcapso.h \
    Models/globalcapso.h \
    Models/swarm.h \
    Models/particle.h \
    Models/latticepoint.h \
    Models/catype.h \
    $$PWD/randomnumber.h

SOURCES += \
    Models/cellularautomaton.cpp \
    Models/localcapso.cpp \
    Models/globalcapso.cpp \
    Models/swarm.cpp \
    Models/particle.cpp \
    Models/latticepoint.cpp \
    $$PWD/randomnumber.cpp