cmake_minimum_required(VERSION 3.5)
project(ss-face LANGUAGES CXX)

add_subdirectory(third_party/QR-Code-generator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (NOT DEFINED QT_VERSION)
    message("QT_VERSION is not defined, default is Qt5. Add -DQT_VERSION=6 to choose Qt6.")
    set(QT_VERSION 5)
endif ()

find_package(Qt${QT_VERSION} COMPONENTS Widgets LinguistTools Network Sql REQUIRED)

set(TS_FILES
    res/i18n/zh_CN.ts)
qt_create_translation(QM_FILES src ${TS_FILES})
add_custom_target(ss-face_ts ALL DEPENDS ${TS_FILES})

add_executable(ss-face WIN32
    res/res.qrc
    src/global.h
    src/main.cpp
    src/version.h
    src/data/entity/SSConfig.h
    src/data/entity/SSConnection.h
    src/data/dao/KeyValueDAO.h
    src/data/dao/SSConfigDAO.h
    src/data/model/ConfigModel.h
    src/data/model/ConnectionModel.h
    src/tools/ProxyLatencyTester.h
    src/tools/SingleInstanceDoorbell.h
    src/tools/QRMatrix.h
    src/ui/custom_widgets/ScalableImageWidget.h
    src/ui/EditDialog.cpp
    src/ui/EditDialog.h
    src/ui/EditDialog.ui
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
    src/ui/MainWindow.ui
    src/ui/ShareDialog.cpp
    src/ui/ShareDialog.h
    src/ui/ShareDialog.ui)

target_include_directories(ss-face PRIVATE src)
target_precompile_headers(ss-face PRIVATE src/pch.h)

target_link_libraries(ss-face PRIVATE
    Qt::Widgets
    Qt::Network
    Qt::Sql
    QR-Code-generator)
