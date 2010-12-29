TEMPLATE = app
TARGET = epubreader

include(../common.pri)

INCLUDEPATH += ../libepub/src ../library-model/src

LIBEPUB = ../libepub/$${QMAKE_PREFIX_STATICLIB}epub.$${QMAKE_EXTENSION_STATICLIB}
LIBLIBRARYMODEL = ../library-model/$${QMAKE_PREFIX_STATICLIB}library-model.$${QMAKE_EXTENSION_STATICLIB}

LIBS += $$LIBEPUB $$LIBLIBRARYMODEL
PRE_TARGETDEPS += $$LIBEPUB $$LIBLIBRARYMODEL

SOURCES = \
    src/main.cxx \
    src/epubaccessmanager.cxx \
    src/epubreply.cxx \
    src/epubview.cxx \
    src/mainwindow.cxx \
    src/settingsdialog.cxx \
    src/epubreaderapplication.cxx \
    src/epublibrarybrowser.cxx

HEADERS = \
    src/epubaccessmanager.h \
    src/epubreply.h \
    src/epubview.h \
    src/mainwindow.h \
    src/settingsdialog.h \
    src/epubreaderapplication.h \
    src/epublibrarybrowser.h

CONFIG += qt warn_on

QT += xmlpatterns gui declarative network webkit dbus


OTHER_FILES += \
    qml/epubreader.qml \
    qml/content/Button.qml \
    qml/content/ScrollBar.qml \
    qml/content/FlickableEPUBView.qml \
    pics/qgn_back_fsm.png \
    pics/qgn_forward_fsm.png \
    pics/general_fullsize.png \
    pics/general_fullsize_b.png \
    desktop/epubreader.desktop \
    icons/epubreader.svg \
    icons/apubreader.png \
    dbus/org.opensource.epubreader.Reader.service \
    qml/content/NavigationButton.qml \
    qml/epublibrary.qml

RESOURCES += \
    epubreader.qrc


FORMS += \
   src/settingsdialog.ui

unix {
    DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

    INSTALLS += target desktop icon48 services

    target.path = $$BINDIR

    desktop.path = $$DATADIR/applications/hildon
    desktop.files += desktop/$${TARGET}.desktop

    icon48.path = $$DATADIR/icons
    icon48.files += icons/$${TARGET}.png

    services.path = $$DBUSDIR
    services.files += dbus/org.opensource.epubreader.Reader.service
}
