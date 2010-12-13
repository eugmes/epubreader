TEMPLATE = app
TARGET = extractor

include(../common.pri)

INCLUDEPATH += ../libepub/src

LIBEPUB = ../libepub/$${QMAKE_PREFIX_STATICLIB}epub.$${QMAKE_EXTENSION_STATICLIB}

LIBS += $$LIBEPUB
PRE_TARGETDEPS += $$LIBEPUB

CONFIG += qt warn_on

QT -= gui
QT += xmlpatterns dbus

SOURCES += \
    src/epubmetadataadaptor.cxx \
    src/main.cxx \
    src/epubextractorapplication.cxx

HEADERS += \
    src/epubmetadataadaptor.h \
    src/epubextractorapplication.h

OTHER_FILES += \
    dbus/org.opensource.epubreader.MetadataExtractor.service

unix {
    INSTALLS = target services

    target.path = $$PKGLIBDIR

    services.path = $$DBUSDIR
    services.files = dbus/org.opensource.epubreader.MetadataExtractor.service
}
