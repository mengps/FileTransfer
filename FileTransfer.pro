QT += quick widgets network concurrent

CONFIG += c++11

RC_ICONS += image/winIcon.ico

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    src/fileapi.h \
    src/fileblock.h \
    src/filetransfer.h \
    src/filemanager.h \
    src/discoverconnection.h \
    src/framelesswindow.h \
    src/transfersocket.h \
    src/connetiomanager.h \
    src/scanneritem.h

SOURCES += \
    src/fileapi.cpp \
    src/fileblock.cpp \
    src/filetransfer.cpp \
    src/framelesswindow.cpp \
    src/main.cpp \
    src/filemanager.cpp \
    src/discoverconnection.cpp \
    src/transfersocket.cpp \
    src/connetiomanager.cpp \
    src/scanneritem.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
