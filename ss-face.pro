QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11
PRECOMPILED_HEADER = src/pch.hpp

SOURCES += \
    src/net/latencytester.cpp \
    src/main.cpp \
    src/model/config.cpp \
    src/manager/configmanager.cpp \
    src/manager/processmanager.cpp \
    src/singleinstance.cpp \
    src/ui/mainwindow.cpp \
    src/ui/editdialog.cpp \
    src/ui/sharedialog.cpp \
    src/qrlib/qrcode.c

HEADERS += \
    src/net/latencytester.h \
    src/model/config.h \
    src/manager/configmanager.h \
    src/manager/processmanager.h \
    src/singleinstance.h \
    src/ui/mainwindow.h \
    src/ui/editdialog.h \
    src/ui/sharedialog.h \
    src/ui/imagewidget.h \
    src/qrlib/qrcode.h \
    src/qrlib/qreasy.h \
    src/version.h

INCLUDEPATH += \
    src/

FORMS += \
    src/ui/mainwindow.ui \
    src/ui/editdialog.ui \
    src/ui/sharedialog.ui

TRANSLATIONS += \
    src/res/translations/zh_CN.ts

RESOURCES += \
    src/res/res.qrc
