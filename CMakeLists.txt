cmake_minimum_required(VERSION 3.19)

project(smand VERSION 1.0.0 LANGUAGES CXX)

# Qt6 要求至少 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 自动处理 Qt 的 moc / rcc / uic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(smand
    main.cpp
    icons.h
    icons.cpp
    appdata.h
    appdata.cpp
    launcherpage.h
    launcherpage.cpp
    mainwindow.h
    mainwindow.cpp
)

target_link_libraries(smand PRIVATE Qt6::Widgets)

# 源码包含中文和 emoji，必须告诉 MSVC 按 UTF-8 编译
if(MSVC)
    target_compile_options(smand PRIVATE /utf-8)
endif()

# 编译成窗口程序（不显示控制台黑窗口）
set_target_properties(smand PROPERTIES WIN32_EXECUTABLE TRUE)
