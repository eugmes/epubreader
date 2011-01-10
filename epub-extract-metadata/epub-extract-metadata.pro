TEMPLATE = app
TARGET = epub-extract-metadata

include(../common.pri)

INCLUDEPATH += ../libepub/src

LIBEPUB = ../libepub/$${QMAKE_PREFIX_STATICLIB}epub.$${QMAKE_EXTENSION_STATICLIB}

LIBS += $$LIBEPUB -lz
PRE_TARGETDEPS += $$LIBEPUB

CONFIG += qt warn_on

QT -= gui
QT += xmlpatterns

SOURCES = \
    src/main.cxx \
    src/epubmetadataparser.cxx \
    src/epubabstractxmlparser.cxx

unix {
    INSTALLS = target

    target.path = $$BINDIR
}

HEADERS += \
    src/epubmetadataparser.h \
    src/epubabstractxmlparser.h
