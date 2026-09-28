#include "mainwindow.h"

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
QLabel#pageTitle {
    font-size: 20px;
    font-weight: 600;
    color: #1f1f1f;
}
QScrollArea {
    background: transparent;
    border: none;
}
QWidget#pageContent {
    background: transparent;
}
QToolButton {
    background: #ffffff;
    border: 1px solid #e6e6e6;
    border-radius: 8px;
    font-size: 13px;
    color: #1f1f1f;
    padding: 6px;
}
QToolButton:hover {
    background: #f0f7fd;
    border: 1px solid #9fd0f9;
}
QToolButton:pressed {
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
    app.setApplicationVersion(QStringLiteral("1.0.0"));
    app.setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
    app.setStyleSheet(QString::fromUtf8(kStyleSheet));

    MainWindow window;
    window.show();

    return app.exec();
}
