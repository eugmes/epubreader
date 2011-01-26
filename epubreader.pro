TEMPLATE = subdirs

SUBDIRS = \
    libepub \
    epub-extract-metadata \
    thumbnailer \
    reader \
    extract-epub \
    library-model \
    translations/translations.pro

thumbnailer.depends = libepub
epub-extract-metadata.depends = libepub
reader.depends = libepub library-model

OTHER_FILES += \
    common.pri \
    COMPONENTS \
    COPYING \
    GPL-3

# Needed for lupdate
TRANSLATIONS += \
    translations/uk.ts
