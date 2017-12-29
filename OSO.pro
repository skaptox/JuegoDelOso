#-------------------------------------------------
#
# Project created by QtCreator 2017-06-22T17:46:01
#
#-------------------------------------------------

QT       += core gui multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OSO
TEMPLATE = app

CONFIG += c++14


SOURCES += main.cpp\
        gamewindow.cpp \
    game.cpp \
    ai.cpp \
    board.cpp \
    agent.cpp

HEADERS  += gamewindow.h \
    game.h \
    ai.h \
    board.h \
    qt_functions.h \
    agent.h

RESOURCES += \
    oso.qrc

DISTFILES +=
