TEMPLATE = app
CONFIG += console c++98
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    cmn/failures.c \
    parser/reader.c \
    parser/parse.c \
    process/execute.c

HEADERS += \
    cmn/failures.h \
    parser/reader.h \
    parser/parse.h \
    process/execute.h

