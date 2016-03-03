TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXX = mpicxx
QMAKE_LINK = mpicxx
QMAKE_CXXFLAGS += -std=c++11

SOURCES += \ 
	mpi_hello_world.cpp

HEADERS += 

