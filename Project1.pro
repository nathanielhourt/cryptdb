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
    databaseclient.cpp

HEADERS += \
    databaseserver.hpp \
    databaseclient.hpp \
    database.hpp
