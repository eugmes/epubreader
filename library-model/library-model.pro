TEMPLATE = lib
TARGET = library-model

include(../common.pri)

CONFIG += qt warn_on staticlib dbusinterfaces

QT += dbus declarative

INCLUDEPATH += ./src .

# FIXME
QMAKE_QDBUSXML2CPP = qdbusxml2cpp -i dbusdefs.h

DBUS_INTERFACES = \
    src/org.freedesktop.Tracker.Search.xml \
    src/org.freedesktop.thumbnailer.xml

OTHER_FILES = \
    src/org.freedesktop.Tracker.Search.xml \
    src/org.freedesktop.thumbnailer.xml

HEADERS += \
    src/dbusdefs.h \
    src/epubdocumentlistmodel.h \
    src/thumbnailerservice.h \
    src/thumbnailitem.h

SOURCES += \
    src/epubdocumentlistmodel.cxx \
    src/thumbnailerservice.cxx \
    src/thumbnailitem.cxx
