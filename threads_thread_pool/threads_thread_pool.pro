TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pthread

SOURCES += \ 
    threads_thread_pool.cpp
	thread_pool.cpp \
	thread_safe_queue.cpp 

HEADERS += \
	task.h \
	thread_pool.h \
	thread_safe_queue.h 

