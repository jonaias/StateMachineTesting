#-------------------------------------------------
#
# Project created by QtCreator 2012-05-19T17:21:30
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = statemachines
CONFIG   += console \
    qtestlib
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    statemachine.cpp \
    transition.cpp

HEADERS += \
    statemachine.h \
    transition.h

OTHER_FILES +=

DESTDIR = "../csharp"

CONFIG(debug, debug|release) {
QMAKE_CXXFLAGS_DEBUG += -g3 -O0 -Wall
message("DEBUG!")
} else {
DEFINES += QT_NO_DEBUG_OUTPUT
QMAKE_CXXFLAGS_RELEASE += -O2
message("RELEASE!")
}
