INCLUDEPATH += $$PWD \

HEADERS += \
    Controller/controller.h \
    Controller/localsettingsdialog.h \
    Controller/globalsettingsdialog.h \
    Controller/batchdialog.h \
    Controller/batchitem.h

SOURCES += \
    Controller/controller.cpp \
    Controller/localsettingsdialog.cpp \
    Controller/globalsettingsdialog.cpp \
    Controller/batchdialog.cpp \
    Controller/batchitem.cpp

FORMS += \
    Controller/controller.ui \
    Controller/localsettingsdialog.ui \
    Controller/globalsettingsdialog.ui \
    Controller/batchdialog.ui \

RESOURCES += \
    Controller/controller.qrc