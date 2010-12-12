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
    src/epubtrackerapplication.cxx \
    src/epubthumbnaileradaptor.cxx \
    src/epubmetadataadaptor.cxx

HEADERS += \
    src/epubtrackerapplication.h \
    src/epubthumbnaileradaptor.h \
    src/epubmetadataadaptor.h

OTHER_FILES += \
    dbus/org.opensource.epubreader.Thumbnailer1.service \
    reg/org.opensource.epubreader.Thumbnailer1.service \
    dbus/org.opensource.epubreader.MetadataExtractor.service
