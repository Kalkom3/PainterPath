QT += core gui printsupport serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

INCLUDEPATH += $$PWD/Include \
               $$PWD/Src \
               $$PWD/UI


SOURCES += Src/comsettingsview.cpp \
           Src/dimensiondata.cpp \
           Src/dimensionsinput.cpp \
           Src/main.cpp \
           Src/mainwindow.cpp \
           Src/pathplanner.cpp \
           Src/scribblearea.cpp \
           Src/menuBar.cpp \
           Src/serialcom.cpp \
           Src/imageinterpreter.cpp
HEADERS += Include/comsettingsview.h \
            Include/dimensiondata.h \
            Include/dimensionsinput.h \
            Include/mainwindow.h \
            Include/pathplanner.h \
            Include/scribblearea.h \
            Include/menuBar.h \
            Include/serialcom.h \
            Include/imageinterpreter.h
FORMS += UI/mainwindow.ui \
         UI/comsettingsview.ui \
         UI/dimensionsinput.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


