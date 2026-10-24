#include "aboutpage.h"

#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

AboutPage::AboutPage(QWidget *parent)
    : QWidget(parent)
{
    // 应用 Logo（资源内嵌真图，256 源图平滑缩放到 96 显示）
    auto *logo = new QLabel(this);
    logo->setPixmap(QPixmap(QStringLiteral(":/logo/logo@5x.png"))
                        .scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logo->setAlignment(Qt::AlignCenter);

    // 应用名 + 版本
    auto *name = new QLabel(QStringLiteral("smand"), this);
    name->setObjectName(QStringLiteral("aboutName"));
    name->setAlignment(Qt::AlignCenter);

    auto *version = new QLabel(
        QStringLiteral("版本 %1 · 系统快捷管理器").arg(qApp->applicationVersion()),
        this);
    version->setObjectName(QStringLiteral("aboutVersion"));
    version->setAlignment(Qt::AlignCenter);

    // 功能介绍（富文本，邮箱为可点击的 mailto 链接）
    auto *text = new QLabel(this);
    text->setObjectName(QStringLiteral("aboutText"));
    text->setWordWrap(true);
    text->setOpenExternalLinks(true);
    text->setText(QStringLiteral(
        "<p style='text-align:center;'>把常用的系统入口聚合到一处，轻量、免安装、即点即达。</p>"
        "<p><b>功能一览</b></p>"
        "<ul style='margin-left:18px; padding:0;'>"
        "<li><b>系统设置</b> —— 一键打开 Windows 设置的显示、声音、网络、账户、更新等页面</li>"
        "<li><b>管理工具</b> —— 快速启动服务、任务管理器、设备管理器、注册表等管理窗口</li>"
        "<li><b>快捷操作</b> —— 控制面板、命令行、此电脑，以及锁屏、关机、重启等常用操作</li>"
        "<li><b>安装应用</b> —— 自动扫描本机安装的应用并显示真实图标，双击即可打开</li>"
        "<li><b>视图切换</b> —— 右上角按钮可在宫格 / 列表两种布局间自由切换</li>"
        "<li><b>托盘常驻</b> —— 关闭窗口时隐藏到系统托盘，双击托盘恢复，右键托盘可退出</li>"
        "</ul>"
        "<p>作者：<b>@Hailen</b><br/>"
        "联系邮箱：<a href='mailto:759430324@qq.com'>759430324@qq.com</a></p>"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(48, 36, 48, 24);
    layout->setSpacing(8);
    layout->addWidget(logo);
    layout->addWidget(name);
    layout->addWidget(version);
    layout->addSpacing(12);
    layout->addWidget(text);
    layout->addStretch(1);
}
