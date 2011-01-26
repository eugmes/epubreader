include(../common.pri)

TEMPLATE = lib
TARGET = Dummy

TS_FILES += \
    uk.ts

OTHER_FILES += $$TS_FILES

lrelease.commands = lrelease ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT}
lrelease.input = TS_FILES
lrelease.output = ${QMAKE_FILE_BASE}.qm
lrelease.CONFIG = no_link target_predeps
QMAKE_EXTRA_COMPILERS += lrelease

for(f, TS_FILES):QM_FILES += $${OUT_PWD}/$$replace(f, \\.ts$, .qm)

unix {
    ts.files = $$QM_FILES
    ts.path = $$TRANSLATIONSDIR
    ts.CONFIG += no_check_exist

    INSTALLS += ts
}
