TEMPLATE = subdirs

SUBDIRS = \
    libepub \
    epub-extract-metadata \
    thumbnailer \
    reader \
    extract-epub

thumbnailer.depends = libepub
epub-extract-metadata.depends = libepub
reader.depends = libepub

OTHER_FILES += \
    common.pri
