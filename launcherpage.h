#pragma once

#include <QWidget>

#include "appdata.h"

class QToolButton;

// 通用"按钮网格"内容页：左侧每个菜单分组对应一个实例
class LauncherPage : public QWidget
{
    Q_OBJECT

public:
    explicit LauncherPage(const QString &title,
                          const QList<ActionItem> &items,
                          QWidget *parent = nullptr);

signals:
    // 通知主窗口在状态栏显示执行反馈
    void actionTriggered(const QString &message);

private:
    QToolButton *createButton(const ActionItem &item);
    void executeItem(const ActionItem &item);
};
