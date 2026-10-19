#pragma once

#include "listpage.h"

// 已安装应用页：扫描注册表列出本机安装的应用（含真实 exe 图标），双击打开
class InstalledAppsPage : public ListPage
{
    Q_OBJECT

public:
    explicit InstalledAppsPage(QWidget *parent = nullptr);

    // 首次进入该页面时才扫描，避免拖慢程序启动
    void ensureLoaded();

private:
    void scan();

    bool m_loaded = false;
};
