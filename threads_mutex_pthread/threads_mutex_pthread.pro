TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_LIBS += -lpthread

SOURCES += \ 
    threads_mutex_pthread.cpp

HEADERS += 

