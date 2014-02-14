#/*
# *  Copyright 2014 Pierre Walch
# *  Website : www.pwalch.net
# *
# *  Avatars is free software: you can redistribute it and/or modify
# *  it under the terms of the GNU General Public License as published by
# *  the Free Software Foundation, either version 3 of the License, or
# *  (at your option) any later version.

# *  Avatars is distributed in the hope that it will be useful,
# *  but WITHOUT ANY WARRANTY; without even the implied warranty of
# *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# *  GNU General Public License for more details.

# *  You should have received a copy of the GNU General Public License
# *  along with Avatars.  If not, see <http://www.gnu.org/licenses/>.
# */

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += -Werror "-Wno-unused-parameter"
#QMAKE_CXXFLAGS += -std=c++11

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
    src/vectorsequence.h \
    src/itimeable.h

FORMS    += src/mainwindow.ui

OTHER_FILES += \
    COPYING.txt
