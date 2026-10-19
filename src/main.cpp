#include "mainwindow.h"

#include "icons.h"

#include <QApplication>
#include <QStyleFactory>

// 全局样式表：浅色简洁风格（仿 Win11 设置页观感）
static const char *kStyleSheet = R"(
QListWidget#sideMenu {
    background: #f3f3f3;
    border-right: 1px solid #e3e3e3;
    outline: 0;
    font-size: 14px;
}
QListWidget#sideMenu::item {
    height: 40px;
    margin: 3px 8px;
    padding: 0 12px;
    border-radius: 6px;
    color: #1f1f1f;
}
QListWidget#sideMenu::item:hover {
    background: #e7e7e7;
}
QListWidget#sideMenu::item:selected {
    background: #cfe4f8;
    color: #005fb8;
    font-weight: 600;
}
QWidget#topBar {
    background: #ffffff;
    border-bottom: 1px solid #ededed;
    min-height: 42px;
}
QToolButton#viewToggle {
    background: #ffffff;
    border: 1px solid #e0e0e0;
    border-radius: 6px;
    font-size: 13px;
    color: #1f1f1f;
    padding: 5px 12px;
}
QToolButton#viewToggle:hover {
    background: #f0f7fd;
    border: 1px solid #9fd0f9;
}
QToolButton#viewToggle:pressed {
    background: #e1effb;
}
QLabel#pageTitle {
    font-size: 20px;
    font-weight: 600;
    color: #1f1f1f;
}
QListWidget#itemList {
    background: transparent;
    border: none;
    outline: 0;
    font-size: 13px;
    color: #1f1f1f;
}
QListWidget#itemList::item {
    border: 1px solid transparent;
    border-radius: 8px;
    padding: 4px;
}
QListWidget#itemList::item:hover {
    background: #f0f7fd;
    border: 1px solid #9fd0f9;
}
QListWidget#itemList::item:selected {
    background: #e1effb;
    border: 1px solid #9fd0f9;
}
QListWidget#itemList::item:selected:active {
    background: #dbeafb;
}
QLabel#aboutName {
    font-size: 32px;
    font-weight: 700;
    color: #1f1f1f;
}
QLabel#aboutVersion {
    font-size: 13px;
    color: #8a8a8a;
}
QLabel#aboutText {
    font-size: 14px;
    color: #333333;
    line-height: 1.6;
}
QMenu {
    background: #ffffff;
    border: 1px solid #e0e0e0;
    padding: 4px;
}
QMenu::item {
    padding: 6px 24px;
    font-size: 13px;
    color: #1f1f1f;
}
QMenu::item:selected {
    background: #e1effb;
}
QStatusBar {
    color: #666666;
}
)";

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("smand"));
    app.setOrganizationName(QStringLiteral("Hailen"));
    app.setApplicationVersion(QStringLiteral("1.1.0"));
    app.setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
    app.setStyleSheet(QString::fromUtf8(kStyleSheet));

    // 应用图标（托盘 / 任务栏 / 标题栏共用）
    app.setWindowIcon(QIcon(Icons::emoji(QStringLiteral("🧰"), 64)));

    // 关闭窗口 = 隐藏到托盘，程序只能从托盘菜单退出
    app.setQuitOnLastWindowClosed(false);

    MainWindow window;
    window.show();

    return app.exec();
}
