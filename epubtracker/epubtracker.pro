TEMPLATE = app
TARGET = epubtracker

include(../common.pri)

INCLUDEPATH += ../libepub/src

LIBEPUB = ../libepub/$${QMAKE_PREFIX_STATICLIB}epub.$${QMAKE_EXTENSION_STATICLIB}

LIBS += $$LIBEPUB
PRE_TARGETDEPS += $$LIBEPUB

CONFIG += qt warn_on

QT -= gui
QT += xmlpatterns dbus

SOURCES += \
    src/main.cxx \
    src/epubtrackerapplication.cxx

HEADERS += \
    src/epubtrackerapplication.h
