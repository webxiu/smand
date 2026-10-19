#pragma once

#include <QMainWindow>

#include "listpage.h"

class InstalledAppsPage;
class QListWidget;
class QStackedWidget;
class QSystemTrayIcon;
class QToolButton;

// 主窗口：左侧菜单 + 右侧内容页（右上角带布局切换按钮，关闭时最小化到系统托盘）
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    // 点关闭按钮 → 隐藏到托盘，而不是退出
    void closeEvent(QCloseEvent *event) override;

private slots:
    void showMessage(const QString &message);
    void toggleViewMode();
    void onMenuRowChanged(int row);
    void showFromTray();

private:
    void addMenuEntry(const QString &title, const QString &emoji, QWidget *page);
    void applyViewMode();
    void updateViewToggleVisual();
    void createTrayIcon();

    QListWidget *m_menu = nullptr;      // 左侧菜单
    QStackedWidget *m_stack = nullptr;  // 右侧内容区
    QToolButton *m_viewToggle = nullptr; // 右上角布局切换按钮
    QSystemTrayIcon *m_tray = nullptr;  // 系统托盘
    InstalledAppsPage *m_appsPage = nullptr;

    ListPage::ViewMode m_viewMode = ListPage::ViewMode::Grid;
    bool m_trayHintShown = false; // 托盘气泡提示只显示一次
};
