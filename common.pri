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
