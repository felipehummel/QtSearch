#-------------------------------------------------
#
# Project created by QtCreator 2011-11-26T12:04:40
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = QtSearch
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    index/memoryindex.cpp \
    index/indexer.cpp \
    query/queryprocessor.cpp

HEADERS += \
    index/index.h \
    index/memoryindex.h \
    index/indexer.h \
    query/queryprocessor.h








