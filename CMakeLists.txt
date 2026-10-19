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
    main.cpp
    icons.h
    icons.cpp
    appdata.h
    appdata.cpp
    listpage.h
    listpage.cpp
    launcherpage.h
    launcherpage.cpp
    installedappspage.h
    installedappspage.cpp
    aboutpage.h
    aboutpage.cpp
    mainwindow.h
    mainwindow.cpp
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(smand ${SMAND_SOURCES})
else()
    add_executable(smand ${SMAND_SOURCES})
endif()

target_link_libraries(smand PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# 源码包含中文和 emoji，必须告诉 MSVC 按 UTF-8 编译
if(MSVC)
    target_compile_options(smand PRIVATE /utf-8)
endif()

# 编译成窗口程序（不显示控制台黑窗口）
set_target_properties(smand PROPERTIES WIN32_EXECUTABLE TRUE)
