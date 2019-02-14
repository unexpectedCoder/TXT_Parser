TARGET = TXT_Parser

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    abstract_txt_parser.cpp

HEADERS += \
    abstract_txt_parser.h
