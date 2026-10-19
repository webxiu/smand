#include "mainwindow.h"

#include "aboutpage.h"
#include "appdata.h"
#include "icons.h"
#include "installedappspage.h"
#include "launcherpage.h"

#include <QApplication>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QListWidget>
#include <QMenu>
#include <QSettings>
#include <QStackedWidget>
#include <QStatusBar>
#include <QSystemTrayIcon>
#include <QToolButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("smand - 系统快捷管理器"));
    resize(1180, 760);

    // ===== 读取上次的视图偏好 =====
    QSettings settings(QStringLiteral("Hailen"), QStringLiteral("smand"));
    m_viewMode = static_cast<ListPage::ViewMode>(
        settings.value(QStringLiteral("viewMode"), static_cast<int>(ListPage::ViewMode::Grid)).toInt());

    // ===== 中央布局：左边菜单，右边（顶栏 + 内容区） =====
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

    // 右上角顶栏：右侧放布局切换按钮
    m_viewToggle = new QToolButton(this);
    m_viewToggle->setObjectName(QStringLiteral("viewToggle"));
    m_viewToggle->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_viewToggle->setCursor(Qt::PointingHandCursor);
    m_viewToggle->setIconSize(QSize(18, 18));
    connect(m_viewToggle, &QToolButton::clicked, this, &MainWindow::toggleViewMode);

    auto *topBar = new QWidget(central);
    topBar->setObjectName(QStringLiteral("topBar"));
    auto *topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(0, 0, 12, 0);
    topLayout->addStretch(1);
    topLayout->addWidget(m_viewToggle);

    // 右侧内容区：每个菜单项一个页面
    m_stack = new QStackedWidget(central);

    auto *right = new QWidget(central);
    auto *rightLayout = new QVBoxLayout(right);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);
    rightLayout->addWidget(topBar);
    rightLayout->addWidget(m_stack, 1);

    rootLayout->addWidget(m_menu);
    rootLayout->addWidget(right, 1);

    setCentralWidget(central);

    // ===== 生成菜单和页面：静态三分组 + 安装应用 + 关于 =====
    const QList<MenuEntry> menus = AppData::menus();
    for (const MenuEntry &entry : menus) {
        auto *page = new LauncherPage(entry.title, entry.items, this);
        connect(page, &ListPage::actionTriggered, this, &MainWindow::showMessage);
        addMenuEntry(entry.title, entry.emoji, page);
    }

    m_appsPage = new InstalledAppsPage(this);
    connect(m_appsPage, &ListPage::actionTriggered, this, &MainWindow::showMessage);
    addMenuEntry(QStringLiteral("安装应用"), QStringLiteral("📱"), m_appsPage);

    addMenuEntry(QStringLiteral("关于"), QStringLiteral("ℹ️"), new AboutPage(this));

    // 左侧选中项变化时切换右侧页面
    connect(m_menu, &QListWidget::currentRowChanged,
            this, &MainWindow::onMenuRowChanged);

    m_menu->setCurrentRow(0); // 默认选中第一项

    applyViewMode(); // 初始化各页面视图 + 切换按钮外观
    createTrayIcon();

    statusBar()->showMessage(QStringLiteral("就绪：点击条目打开对应窗口，双击托盘图标可恢复窗口"));
}

void MainWindow::addMenuEntry(const QString &title, const QString &emoji, QWidget *page)
{
    auto *menuItem = new QListWidgetItem(Icons::emoji(emoji, 24), title, m_menu);
    menuItem->setSizeHint(QSize(200, 44));
    menuItem->setToolTip(QStringLiteral("切换到「%1」").arg(title));
    m_stack->addWidget(page);
}

void MainWindow::onMenuRowChanged(int row)
{
    m_stack->setCurrentIndex(row);

    // 首次进入"安装应用"页时才扫描注册表
    if (m_stack->widget(row) == m_appsPage) {
        m_appsPage->ensureLoaded();
    }

    // 切换按钮只对列表类页面有意义，"关于"页隐藏
    const bool isListPage = qobject_cast<ListPage *>(m_stack->widget(row)) != nullptr;
    m_viewToggle->setVisible(isListPage);
}

void MainWindow::toggleViewMode()
{
    m_viewMode = (m_viewMode == ListPage::ViewMode::Grid)
                     ? ListPage::ViewMode::List
                     : ListPage::ViewMode::Grid;

    // 记住偏好，下次启动沿用
    QSettings settings(QStringLiteral("Hailen"), QStringLiteral("smand"));
    settings.setValue(QStringLiteral("viewMode"), static_cast<int>(m_viewMode));

    applyViewMode();
}

void MainWindow::applyViewMode()
{
    for (int i = 0; i < m_stack->count(); ++i) {
        if (auto *page = qobject_cast<ListPage *>(m_stack->widget(i))) {
            page->setViewMode(m_viewMode);
        }
    }
    updateViewToggleVisual();
}

void MainWindow::updateViewToggleVisual()
{
    // 按钮显示"将要切换到"的视图，点击后生效
    const bool isGrid = (m_viewMode == ListPage::ViewMode::Grid);
    m_viewToggle->setIcon(Icons::emoji(isGrid ? QStringLiteral("📋") : QStringLiteral("🗃️"), 24));
    m_viewToggle->setText(isGrid ? QStringLiteral("列表视图") : QStringLiteral("宫格视图"));
    m_viewToggle->setToolTip(isGrid ? QStringLiteral("切换为列表布局")
                                    : QStringLiteral("切换为宫格布局"));
}

void MainWindow::createTrayIcon()
{
    m_tray = new QSystemTrayIcon(windowIcon(), this);
    m_tray->setToolTip(QStringLiteral("smand - 系统快捷管理器"));

    auto *menu = new QMenu(this);
    QAction *showAction = menu->addAction(QStringLiteral("显示主界面"));
    connect(showAction, &QAction::triggered, this, &MainWindow::showFromTray);
    menu->addSeparator();
    QAction *quitAction = menu->addAction(QStringLiteral("退出"));
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    m_tray->setContextMenu(menu);

    // 单击 / 双击托盘图标 → 显示主窗口
    connect(m_tray, &QSystemTrayIcon::activated, this,
            [this](QSystemTrayIcon::ActivationReason reason) {
                if (reason == QSystemTrayIcon::Trigger
                    || reason == QSystemTrayIcon::DoubleClick) {
                    showFromTray();
                }
            });

    m_tray->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 系统支持托盘时，关闭按钮 = 隐藏到托盘
    if (m_tray && m_tray->isVisible() && QSystemTrayIcon::isSystemTrayAvailable()) {
        hide();
        event->ignore();

        // 首次隐藏时气泡提示一次
        if (!m_trayHintShown) {
            m_trayHintShown = true;
            m_tray->showMessage(
                QStringLiteral("smand 仍在运行"),
                QStringLiteral("程序已最小化到系统托盘：双击托盘图标可重新打开，右键托盘图标可选择退出。"));
        }
    } else {
        event->accept();
    }
}

void MainWindow::showFromTray()
{
    show();
    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    raise();
    activateWindow();
}

void MainWindow::showMessage(const QString &message)
{
    // 显示 6 秒后自动清除
    statusBar()->showMessage(message, 6000);
}
