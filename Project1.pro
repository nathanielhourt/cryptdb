#-------------------------------------------------
#
# Project created by QtCreator 2014-02-16T14:25:38
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Project1
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG  += crypto c++11

SOURCES += main.cpp \
    databaseserver.cpp \
    databaseclient.cpp \
    crypto.cpp \
    paillierprivatekey.cpp \
    paillierpublickey.cpp \
    modularmath.cpp \
    bigintmath.cpp

HEADERS += \
    databaseserver.hpp \
    databaseclient.hpp \
    database.hpp \
    crypto.hpp \
    modularmath.hpp \
    paillierpublickey.hpp \
    paillierprivatekey.hpp \
    bigintmath.hpp

osx: INCLUDEPATH += /opt/local/include
osx: QMAKE_CXXFLAGS += /opt/local/lib/libgmp.dylib
linux|win32: LIBS += -lgmp
