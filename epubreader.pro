TEMPLATE = subdirs

SUBDIRS = \
    libepub \
    thumbnailer \
    extractor \
    reader

thumbnailer.depends = libepub
extractor.depends = libepub
reader.depends = libepub

OTHER_FILES += \
    common.pri
