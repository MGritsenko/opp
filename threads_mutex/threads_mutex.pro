TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pthread

SOURCES += \ 
    threads_mutex.cpp

HEADERS +=

