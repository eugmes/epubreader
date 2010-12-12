TEMPLATE = lib
TARGET = epub

CONFIG += qt warn_on staticlib

QT -= gui
QT += xmlpatterns

SOURCES = \
    src/zipreader.cxx \
    src/epubfile.cxx

HEADERS = \
    src/zipreader.h \
    src/epubfile.h
