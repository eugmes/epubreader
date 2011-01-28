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

PO_FILES += \
    uk.po

OTHER_FILES += $$PO_FILES

msgfmt.commands = msgfmt -c --statistics ${QMAKE_FILE_IN} -o ${QMAKE_FILE_OUT}
msgfmt.input = PO_FILES
msgfmt.output = ${QMAKE_FILE_BASE}.mo
msgfmt.CONFIG = no_link target_predeps
QMAKE_EXTRA_COMPILERS += msgfmt

for(f, PO_FILES):MO_FILES += $${OUT_PWD}/$$replace(f, \\.po$, .mo)

OTHER_FILES += \
    epubreader.pot

unix {
    ts.files = $$QM_FILES
    ts.path = $$TRANSLATIONSDIR
    ts.CONFIG += no_check_exist
    INSTALLS += ts

    install_mo.commands = $$QMAKE_MKDIR ${INSTALL_ROOT}$${LOCALE_PATH}/${QMAKE_FILE_BASE}/LC_MESSAGES && \
        $$QMAKE_INSTALL_FILE ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
    install_mo.input = MO_FILES
    install_mo.output = ${INSTALL_ROOT}$${LOCALE_PATH}/${QMAKE_FILE_BASE}/LC_MESSAGES/epubreader.mo
    install_mo.depends += FORCE
    install_mo.CONFIG = no_link no_clean
    QMAKE_EXTRA_COMPILERS += install_mo

    install_mo_dummy.depends += compiler_install_mo_make_all
    install_mo_dummy.CONFIG += dummy_install
    INSTALLS += install_mo_dummy
}
