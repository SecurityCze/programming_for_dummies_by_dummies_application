QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    csettingsstorage.cpp \
    ctaskstorage.cpp \
    main.cpp \
    mainmenu.cpp \
    settings.cpp \
    task.cpp \
    taskchooser.cpp

HEADERS += \
    csettingsstorage.h \
    ctaskstorage.h \
    mainmenu.h \
    settings.h \
    task.h \
    taskchooser.h

FORMS += \
    mainmenu.ui \
    settings.ui \
    task.ui \
    taskchooser.ui

TRANSLATIONS += \
    Programming_for_dummies_by_dummies_application_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

# lib2git static library
# https://github.com/libgit2/libgit2
unix:!macx|win32: LIBS += -L$$PWD/external/ -lgit2

INCLUDEPATH += $$PWD/external
DEPENDPATH += $$PWD/external

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/external/git2.lib
#else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$PWD/external/libgit2.a
