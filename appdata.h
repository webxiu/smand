#pragma once

#include <QList>
#include <QString>

// 一条操作项：按钮上显示什么 + 点击后执行什么
struct ActionItem {
    QString title;         // 按钮文字
    QString emoji;         // 按钮图标（emoji）
    QString command;       // 命令内容，统一按 "cmd /c <command>" 执行
    QString tip;           // 提示语（状态栏 / 悬浮提示）
    bool confirm = false;  // 危险操作需要二次确认
};

// 左侧一个菜单分组
struct MenuEntry {
    QString title;
    QString emoji;
    QList<ActionItem> items;
};

namespace AppData {

// 全部菜单分组（按返回顺序显示在左侧菜单）
QList<MenuEntry> menus();

} // namespace AppData
