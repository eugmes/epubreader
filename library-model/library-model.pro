TEMPLATE = lib
TARGET = library-model

include(../common.pri)

CONFIG += qt warn_on staticlib dbusinterfaces

QT -= gui
QT += dbus

INCLUDEPATH += ./src .

# FIXME
QMAKE_QDBUSXML2CPP = qdbusxml2cpp -i dbusdefs.h

DBUS_INTERFACES = \
    src/org.freedesktop.Tracker.Search.xml

OTHER_FILES = \
    src/org.freedesktop.Tracker.Search.xml

HEADERS += \
    src/dbusdefs.h \
    src/epubdocumentlistmodel.h

SOURCES += \
    src/epubdocumentlistmodel.cxx
