TEMPLATE = subdirs

SUBDIRS = \
    libepub \
    epubtracker \
    reader

epubtracker.depends = libepub
reader.depends = libepub

OTHER_FILES += \
    common.pri
