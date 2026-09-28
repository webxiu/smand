#pragma once

#include <QMainWindow>

class QListWidget;
class QStackedWidget;

// 主窗口：左侧菜单 + 右侧内容页
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    // 在状态栏显示操作反馈，几秒后自动消失
    void showMessage(const QString &message);

private:
    QListWidget *m_menu = nullptr;    // 左侧菜单
    QStackedWidget *m_stack = nullptr; // 右侧内容区
};
