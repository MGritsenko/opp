TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_LIBS += -lpthread

SOURCES += \ 
    threads_cond_var_pthread.cpp

HEADERS +=

