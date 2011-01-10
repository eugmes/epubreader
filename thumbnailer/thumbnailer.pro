TEMPLATE = app
TARGET = thumbnailer

include(../common.pri)

INCLUDEPATH += ../libepub/src

LIBEPUB = ../libepub/$${QMAKE_PREFIX_STATICLIB}epub.$${QMAKE_EXTENSION_STATICLIB}

LIBS += $$LIBEPUB -lz
PRE_TARGETDEPS += $$LIBEPUB

CONFIG += qt warn_on

QT += xmlpatterns dbus

SOURCES += \
    src/main.cxx \
    src/epubthumbnailerapplication.cxx \
    src/epubthumbnaileradaptor.cxx \
    src/epubthumbnailerrequest.cxx \
    src/epubthumbnailerrequestqueue.cxx

HEADERS += \
    src/epubthumbnailerapplication.h \
    src/epubthumbnaileradaptor.h \
    src/epubthumbnailerrequest.h \
    src/epubthumbnailerrequestqueue.h

OTHER_FILES += \
    dbus/org.opensource.epubreader.Thumbnailer1.service \
    reg/org.opensource.epubreader.Thumbnailer1.service

unix {
    INSTALLS = target services reg

    target.path = $$PKGLIBDIR

    services.path = $$DBUSDIR
    services.files = dbus/org.opensource.epubreader.Thumbnailer1.service

    reg.path = $$DATADIR/thumbnailers
    reg.files = reg/org.opensource.epubreader.Thumbnailer1.service
}
