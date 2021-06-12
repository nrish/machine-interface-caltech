QT      += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 static console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calibrationdialog.cpp \
    imagedialog.cpp \
    main.cpp \
    mainwindow.cpp \
    serialmanager.cpp \
    uint32spinbox.cpp

HEADERS += \
    calibrationdialog.h \
    imagedialog.h \
    mainwindow.h \
    serialData/serialData.h \
    serialmanager.h \
    uint32spinbox.h \
    serialData.h

FORMS += \
    calibrationdialog.ui \
    imagedialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
