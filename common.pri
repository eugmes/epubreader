DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_NO_KEYWORDS QT_NO_URL_CAST_FROM_STRING

QMAKE_CXXFLAGS += -fno-exceptions -fno-rtti -g -O2 -std=c++98 -pedantic -Wno-long-long
QMAKE_LFLAGS += -fno-exceptions -fno-rtti -g -O2 -Wl,--as-needed -Wl,-O1

unix {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }

    BINDIR = $$PREFIX/bin
    LIBDIR = $$PREFIX/lib
    PKGLIBDIR = $$LIBDIR/epubreader
    TRANSLATIONSDIR = $$PREFIX/share/epubreader/translations

    DBUSDIR = /usr/share/dbus-1/services

    LOCALE_PATH=/usr/share/locale
}
