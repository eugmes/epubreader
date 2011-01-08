DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_NO_KEYWORDS

unix {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }

    BINDIR = $$PREFIX/bin
    LIBDIR = $$PREFIX/lib
    PKGLIBDIR = $$LIBDIR/epubreader
    DATADIR = $$PREFIX/share
    PKGDATADIR = $$DATADIR/epubreader

    DBUSDIR = $$DATADIR/dbus-1/services
}
