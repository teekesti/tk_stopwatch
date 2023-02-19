TEMPLATE = app

QT += qml quick
CONFIG += qmltypes
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += ENABLE_FUNC_TRACE ENABLE_TRACE

HEADERS += stopwatchengine.h

SOURCES += main.cpp \
    stopwatchengine.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = .
QML_IMPORT_NAME = com.gmail.kesti.tero.stopwatchengine
QML_IMPORT_MAJOR_VERSION = 1
# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    Button.qml \
    tkstopwatch.png \
    tkstopwatch.svg \
    Doxyfile

HEADERS += \
    eoqttrace.h \
    stopwatchengine.h
