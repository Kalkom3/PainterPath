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
           Src/serialcom.cpp
HEADERS += Include/comsettingsview.h \
            Include/dimensiondata.h \
            Include/dimensionsinput.h \
            Include/mainwindow.h \
            Include/pathplanner.h \
            Include/scribblearea.h \
            Include/menuBar.h \
            Include/serialcom.h
FORMS += UI/mainwindow.ui \
         UI/comsettingsview.ui \
         UI/dimensionsinput.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/build/x64/vc15/lib/ -lopencv_world455
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/build/x64/vc15/lib/ -lopencv_world455d
else:unix: LIBS += -L$$PWD/../../opencv/build/x64/vc15/lib/ -lopencv_world455

INCLUDEPATH += $$PWD/../../opencv/build/include
DEPENDPATH += $$PWD/../../opencv/build/include
