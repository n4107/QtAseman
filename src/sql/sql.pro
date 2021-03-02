load(qt_build_config)

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

TARGET = QAsemanSql
QT = core asemancore


qtHaveModule(sql): QT += sql

MODULE = asemansql

load(qt_module)

DEFINES += LIBQTASEMAN_SQL_LIBRARY

HEADERS += \
    $$PWD/asemansql_global.h \
    $$PWD/asemansqlobject.h

SOURCES += \
    $$PWD/asemansqlobject.cpp
