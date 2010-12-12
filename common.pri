unix {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }

    BINDIR = $$PREFIX/bin
    DATADIR = $$PREFIX/share
    PKGDATADIR = $$DATADIR/epubreader
}
