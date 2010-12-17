TEMPLATE = lib
TARGET = epub

include(../common.pri)

CONFIG += qt warn_on staticlib

QT -= gui
QT += xmlpatterns

SOURCES = \
    src/zipreader.cxx \
    src/epubfile.cxx

HEADERS = \
    src/zipreader.h \
    src/epubfile.h
