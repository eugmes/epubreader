TEMPLATE = app
TARGET = epubtracker

include(../common.pri)

INCLUDEPATH += ../libepub/src

LIBEPUB = ../libepub/$${QMAKE_PREFIX_STATICLIB}epub.$${QMAKE_EXTENSION_STATICLIB}

LIBS += $$LIBEPUB
PRE_TARGETDEPS += $$LIBEPUB

CONFIG += qt warn_on

QT += xmlpatterns dbus

SOURCES += \
    src/main.cxx \
    src/epubtrackerapplication.cxx \
    src/epubthumbnaileradaptor.cxx \
    src/epubmetadataadaptor.cxx \
    src/epubthumbnailerrequest.cxx \
    src/epubthumbnailerrequestqueue.cxx

HEADERS += \
    src/epubtrackerapplication.h \
    src/epubthumbnaileradaptor.h \
    src/epubmetadataadaptor.h \
    src/epubthumbnailerrequest.h \
    src/epubthumbnailerrequestqueue.h

OTHER_FILES += \
    dbus/org.opensource.epubreader.Thumbnailer1.service \
    reg/org.opensource.epubreader.Thumbnailer1.service \
    dbus/org.opensource.epubreader.MetadataExtractor.service

unix {
    INSTALLS = target services thumbnailer

    target.path = $$PKGLIBDIR

    services.path = $$DBUSDIR
    services.files = dbus/org.opensource.epubreader.Thumbnailer1.service

    thumbnailer.path = $$DATADIR/thumbnailers
    thumbnailer.files = reg/org.opensource.epubreader.Thumbnailer1.service
}
