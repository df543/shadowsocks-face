#-------------------------------------------------
#
# Project created by QtCreator 2019-01-15T08:30:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ss-face
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
PRECOMPILED_HEADER = src/pch.hpp

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/editdialog.cpp \
    src/ssconfigstorage.cpp \
    src/ssprocesslist.cpp \
    src/singleinstance.cpp \
    src/qrlib/qrcode.c \
    src/sharedialog.cpp

HEADERS += \
    src/mainwindow.h \
    src/editdialog.h \
    src/ssconfigstorage.h \
    src/ssprocesslist.h \
    src/singleinstance.h \
    src/qrlib/qrcode.h \
    src/qrlib/qreasy.h \
    src/sharedialog.h \
    src/imagewidget.h

INCLUDEPATH += src/

FORMS += \
    src/mainwindow.ui \
    src/editdialog.ui \
    src/sharedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TRANSLATIONS += \
    src/translations/zh_CN.ts

RESOURCES += \
    src/res.qrc
