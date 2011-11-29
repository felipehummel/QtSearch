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
    query/queryprocessor.cpp \
    analyzer/tokenstream.cpp \
    analyzer/analyzer.cpp \
    analyzer/tokenizer.cpp

HEADERS += \
    index/index.h \
    index/memoryindex.h \
    index/indexer.h \
    query/queryprocessor.h \
    analyzer/tokenstream.h \
    analyzer/analyzer.h \
    analyzer/tokenizer.h














