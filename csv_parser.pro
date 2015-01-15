TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c

include(deployment.pri)
qtcAddDeployment()

*gcc*{
QMAKE_CFLAGS += -std=c90 -pedantic -Wall -Wextra -Wconversion \
                -Wsign-conversion -Wunreachable-code -Wshadow
}
