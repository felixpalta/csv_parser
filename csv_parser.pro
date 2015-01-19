TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    csv.c

include(deployment.pri)
qtcAddDeployment()

*gcc*{
QMAKE_CFLAGS += -pedantic -Wall -Wextra -Wconversion \
                -Wsign-conversion -Wunreachable-code -Wshadow
}

HEADERS += \
    csv.h
