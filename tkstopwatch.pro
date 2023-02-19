TEMPLATE = app

QT += qml quick
CONFIG += qmltypes
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += ENABLE_FUNC_TRACE ENABLE_TRACE

HEADERS += \
    eoqttrace.h \
    stopwatchengine.h

SOURCES += main.cpp \
    stopwatchengine.cpp

RESOURCES += qml.qrc

QML_IMPORT_NAME = com.gmail.kesti.tero.stopwatchengine
QML_IMPORT_MAJOR_VERSION = 1

DISTFILES += \
    tkstopwatch.png \
    tkstopwatch.svg \
    Doxyfile


