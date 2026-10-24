cmake_minimum_required(VERSION 3.19)

project(smand VERSION 1.1.0 LANGUAGES CXX)

# C++17（Qt6 要求；Qt5.15 也支持）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 自动处理 Qt 的 moc / rcc / uic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 双版本兼容：优先 Qt6；当 CMAKE_PREFIX_PATH 指向 Qt5 目录时自动用 Qt5
# （Qt 5.15.2 用于老系统兼容包：Win7 SP1 / Win8 / Win10 初版 / Server 2012R2）
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
message(STATUS "使用 Qt${QT_VERSION_MAJOR}（${Qt${QT_VERSION_MAJOR}_DIR}）")

if(QT_VERSION_MAJOR EQUAL 6)
    qt_standard_project_setup()
endif()

set(SMAND_SOURCES
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/pages/listpage.h
    src/pages/listpage.cpp
    src/pages/launcherpage.h
    src/pages/launcherpage.cpp
    src/pages/installedappspage.h
    src/pages/installedappspage.cpp
    src/pages/aboutpage.h
    src/pages/aboutpage.cpp
    src/data/appdata.h
    src/data/appdata.cpp
    src/utils/icons.h
    src/utils/icons.cpp
    src/utils/shell.h
    src/utils/shell.cpp

    # 图片资源清单：logo / 图标经 rcc 编译进 exe 内部（用法见 resources/README.md）
    resources/smand.qrc
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(smand ${SMAND_SOURCES})
else()
    add_executable(smand ${SMAND_SOURCES})
endif()

target_link_libraries(smand PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Windows exe 文件图标（资源管理器 / 桌面快捷方式显示）；
# app.ico 由 scripts/make_ico.py 从 logo 多倍图生成，更换 Logo 后重跑即可
if(WIN32)
    enable_language(RC)
    target_sources(smand PRIVATE resources/smand.rc)
endif()

# 头文件按功能分散在 src 各子目录，统一加入搜索路径，
# 源码内 #include "xxx.h" 无需带相对路径（AUTOMOC 同样依赖此设置）
target_include_directories(smand PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pages
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

# 源码包含中文和 emoji，必须告诉 MSVC 按 UTF-8 编译
if(MSVC)
    target_compile_options(smand PRIVATE /utf-8)
endif()

# 编译成窗口程序（不显示控制台黑窗口）
set_target_properties(smand PROPERTIES WIN32_EXECUTABLE TRUE)
