cmake_minimum_required(VERSION 3.5)
project(ss-face LANGUAGES CXX)

# add_subdirectory(third_party/QR-Code-generator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (NOT DEFINED QT_VERSION)
    message("QT_VERSION is not defined, default is Qt5. Add -DQT_VERSION=6 to choose Qt6.")
    set(QT_VERSION 5)
endif ()

find_package(Qt${QT_VERSION} COMPONENTS Widgets LinguistTools Network REQUIRED)

set(TS_FILES
    res/i18n/zh_CN.ts)
qt_create_translation(QM_FILES src ${TS_FILES})
add_custom_target(ss-face_ts ALL DEPENDS ${TS_FILES})

add_executable(ss-face WIN32
    res/res.qrc
    src/app/global.cpp
    src/app/global.h
    src/app/main.cpp
    src/app/version.h
    src/data/configmanager.cpp
    src/data/configmanager.h
    src/model/ssconfig.cpp
    src/model/ssconfig.h
    src/sspm/processmanager.cpp
    src/sspm/processmanager.h
    src/sspm/sspm.cpp
    src/sspm/sspm.h
    src/third_party/QR-Code-generator/cpp/QrCode.cpp
    src/third_party/QR-Code-generator/cpp/QrCode.hpp
    src/tools/latencytester.cpp
    src/tools/latencytester.h
    src/tools/onceguard.cpp
    src/tools/onceguard.h
    src/tools/qreasy.h
    src/ui/custom/imagewidget.h
    src/ui/editdialog.cpp
    src/ui/editdialog.h
    src/ui/editdialog.ui
    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
    src/ui/mainwindow.ui
    src/ui/sharedialog.cpp
    src/ui/sharedialog.h
    src/ui/sharedialog.ui)

target_include_directories(ss-face PRIVATE src)
target_precompile_headers(ss-face PRIVATE src/pch.h)

target_link_libraries(ss-face PRIVATE
    Qt::Widgets
    Qt::Network)
    # QR-Code-generator)
