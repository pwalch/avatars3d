#-------------------------------------------------
#
# Project created by QtCreator 2013-07-07T23:01:36
#
#-------------------------------------------------

# 3D Avatars, Pierre Walch

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -Werror

TARGET = Avatars
TEMPLATE = app

win32 {
    LIBS += -LC:\Irrlicht\irrlicht-1.7.1\lib\Win32-visualstudio
    INCLUDEPATH += C:\Irrlicht\irrlicht-1.7.1\include
    # Adapt Revel and XviD for Windows
    LIBS += -lIrrlicht
}

unix {
    LIBS += -L/usr/lib/x86_64-linux-gnu/libIrrlicht.a
    INCLUDEPATH += /usr/include/irrlicht
    LIBS += -lIrrlicht

    LIBS += -L/usr/local/lib/librevel.a
    LIBS += -lrevel
    LIBS += -L/usr/lib/x86_64-linux-gnu/libxvidcore.a
    LIBS += -lxvidcore
    #LIBS += -lX11
}



QMAKE_CXXFLAGS+="-Wno-unused-parameter"

SOURCES += src/main.cpp \
        src/mainwindow.cpp \
        src/camerawindow.cpp \
    libs/tinyxml2.cpp \
    src/engine.cpp \
    src/court.cpp \
    src/player.cpp \
    src/eventmanager.cpp \
    src/colorcurvenode.cpp \
    src/moveable.cpp \
    src/movingbody.cpp \
    src/affinetransformation.cpp \
    src/avatarsfactory.cpp \
    src/settingsparser.cpp \
    src/science.cpp \
    src/vectorsequence.cpp

HEADERS += src/mainwindow.h \
        src/camerawindow.h \
        libs/tinyxml2.h \
    src/engine.h \
    src/court.h \
    src/player.h \
    src/eventmanager.h \
    src/colorcurvenode.h \
    src/moveable.h \
    src/movingbody.h \
    src/timeable.h \
    src/camerasettings.h \
    src/affinetransformation.h \
    src/sequencesettings.h \
    src/playersettings.h \
    src/courtsettings.h \
    src/bodysettings.h \
    src/avatarsfactory.h \
    src/settingsparser.h \
    src/actionsettings.h \
    src/science.h \
    src/vectorsequence.h

FORMS    += src/mainwindow.ui

OTHER_FILES +=
