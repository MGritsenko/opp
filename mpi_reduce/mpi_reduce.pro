TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXX = mpicxx
QMAKE_LINK = mpicxx

SOURCES += \ 
	mpi_reduce.cpp

HEADERS += 

