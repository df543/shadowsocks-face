QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11
PRECOMPILED_HEADER = src/common.hpp

SOURCES += \
    src/data/configmanager.cpp \
    src/app/global.cpp \
    src/app/main.cpp \
    src/model/ssconfig.cpp \
    src/sspm/processmanager.cpp \
    src/sspm/sspm.cpp \
    src/third_party/qrcode/src/qrcode.c \
    src/tools/latencytester.cpp \
    src/tools/onceguard.cpp \
    src/tools/qreasy.cpp \
    src/ui/editdialog.cpp \
    src/ui/mainwindow.cpp \
    src/ui/sharedialog.cpp

HEADERS += \
    src/data/configmanager.h \
    src/app/global.h \
    src/app/version.h \
    src/model/ssconfig.h \
    src/sspm/processmanager.h \
    src/sspm/sspm.h \
    src/third_party/qrcode/src/qrcode.h \
    src/tools/onceguard.h \
    src/tools/qreasy.h \
    src/tools/latencytester.h \
    src/ui/editdialog.h \
    src/ui/custom/imagewidget.h \
    src/ui/mainwindow.h \
    src/ui/sharedialog.h

INCLUDEPATH += \
    src/

FORMS += \
    src/ui/mainwindow.ui \
    src/ui/editdialog.ui \
    src/ui/sharedialog.ui

TRANSLATIONS += \
    res/translations/zh_CN.ts

RESOURCES += \
    res/res.qrc
