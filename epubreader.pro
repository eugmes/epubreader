TEMPLATE = subdirs

SUBDIRS = \
    libepub \
    thumbnailer \
    reader

thumbnailer.depends = libepub
reader.depends = libepub

OTHER_FILES += \
    common.pri
