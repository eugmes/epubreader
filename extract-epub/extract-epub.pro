TEMPLATE = lib
TARGET = extract-epub

include(../common.pri)

CONFIG = link_pkgconfig use_c_linker plugin warn_on

PKGCONFIG += glib-2.0

INCLUDEPATH += include

HEADERS += \
    include/tracker-main.h \
    include/tracker-escape.h

SOURCES += \
    tracker-extract-epub.c \
    tracker-escape.c

OTHER_FILES += \
    GPL-2

unix {
    INSTALLS = target

    target.path = $$LIBDIR/tracker/extract-modules
}
