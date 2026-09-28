#include "mainwindow.h"

#include "appdata.h"
#include "icons.h"
#include "launcherpage.h"

#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("smand - 系统快捷管理器"));
    resize(1180, 760);

    // ===== 中央布局：左边菜单，右边内容 =====
    auto *central = new QWidget(this);
    auto *rootLayout = new QHBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // 左侧菜单
    m_menu = new QListWidget(central);
    m_menu->setObjectName(QStringLiteral("sideMenu"));
    m_menu->setFixedWidth(200);
    m_menu->setIconSize(QSize(24, 24));
    m_menu->setFocusPolicy(Qt::NoFocus);
    m_menu->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    // 右侧内容区：每个菜单分组一个页面
    m_stack = new QStackedWidget(central);

    rootLayout->addWidget(m_menu);
    rootLayout->addWidget(m_stack, 1);

    setCentralWidget(central);

    // ===== 根据数据表生成菜单和页面 =====
    const QList<MenuEntry> menus = AppData::menus();
    for (const MenuEntry &entry : menus) {
        auto *page = new LauncherPage(entry.title, entry.items, this);
        connect(page, &LauncherPage::actionTriggered,
                this, &MainWindow::showMessage);

        auto *menuItem = new QListWidgetItem(
            Icons::emoji(entry.emoji, 24), entry.title, m_menu);
        menuItem->setSizeHint(QSize(200, 44));
        menuItem->setToolTip(QStringLiteral("切换到「%1」").arg(entry.title));

        m_stack->addWidget(page);
    }

    // 左侧选中项变化时切换右侧页面
    connect(m_menu, &QListWidget::currentRowChanged,
            m_stack, &QStackedWidget::setCurrentIndex);

    m_menu->setCurrentRow(0); // 默认选中第一项

    statusBar()->showMessage(QStringLiteral("就绪，点击右侧按钮即可打开对应的系统窗口"));
}

void MainWindow::showMessage(const QString &message)
{
    // 显示 6 秒后自动清除
    statusBar()->showMessage(message, 6000);
}
