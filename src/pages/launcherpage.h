#pragma once

#include "appdata.h"
#include "listpage.h"

// 静态功能项页面（系统设置 / 管理工具 / 快捷操作共用）
class LauncherPage : public ListPage
{
    Q_OBJECT

public:
    explicit LauncherPage(const QString &title,
                          const QList<ActionItem> &items,
                          QWidget *parent = nullptr);

private:
    void executeItem(const ActionItem &item);

    QList<ActionItem> m_items;
};
