DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_NO_KEYWORDS

QMAKE_CXXFLAGS += -fno-exceptions -fno-rtti -O2 -std=c++98 -pedantic -Wno-long-long
QMAKE_LFLAGS += -fno-exceptions -fno-rtti -O2 -Wl,--as-needed -Wl,-O1

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
