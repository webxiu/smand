#include "launcherpage.h"

#include "icons.h"

#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QProcess>
#include <QScrollArea>
#include <QToolButton>
#include <QVBoxLayout>

namespace {

// 网格布局参数
constexpr int kColumns      = 4;    // 每行按钮数
constexpr int kButtonWidth  = 150;  // 按钮宽度
constexpr int kButtonHeight = 112;  // 按钮高度
constexpr int kIconSize     = 44;   // 图标尺寸

} // namespace

LauncherPage::LauncherPage(const QString &title,
                           const QList<ActionItem> &items,
                           QWidget *parent)
    : QWidget(parent)
{
    // 页面标题
    auto *titleLabel = new QLabel(title, this);
    titleLabel->setObjectName(QStringLiteral("pageTitle"));

    // 按钮网格（外面包一层滚动区域，窗口太小时可以滚动）
    auto *content = new QWidget;
    content->setObjectName(QStringLiteral("pageContent"));

    auto *grid = new QGridLayout(content);
    grid->setContentsMargins(4, 4, 12, 12);
    grid->setSpacing(10);

    int row = 0;
    int col = 0;
    for (const ActionItem &item : items) {
        grid->addWidget(createButton(item), row, col);
        if (++col == kColumns) {
            col = 0;
            ++row;
        }
    }
    grid->setRowStretch(row + 1, 1); // 最后一行下方留白可伸展

    auto *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setWidget(content);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 18, 24, 8);
    layout->setSpacing(10);
    layout->addWidget(titleLabel);
    layout->addWidget(scroll, 1);
}

QToolButton *LauncherPage::createButton(const ActionItem &item)
{
    auto *button = new QToolButton(this);
    button->setText(item.title);
    button->setIcon(Icons::emoji(item.emoji, kIconSize));
    button->setIconSize(QSize(kIconSize, kIconSize));
    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    button->setFixedSize(kButtonWidth, kButtonHeight);
    button->setCursor(Qt::PointingHandCursor);
    button->setToolTip(item.tip);

    // 结构体按值捕获，点击时使用当时的数据
    connect(button, &QToolButton::clicked, this, [this, item]() {
        executeItem(item);
    });
    return button;
}

void LauncherPage::executeItem(const ActionItem &item)
{
    // 危险操作（关机/重启/注销）先确认
    if (item.confirm) {
        const auto answer = QMessageBox::question(
            this,
            QStringLiteral("确认操作"),
            QStringLiteral("确定要执行「%1」吗？").arg(item.title),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No);
        if (answer != QMessageBox::Yes) {
            return;
        }
    }

    // 统一交给 cmd 执行：支持 start 打开窗口、shutdown 电源命令、rundll32 等
    QProcess::startDetached(QStringLiteral("cmd"),
                            { QStringLiteral("/c"), item.command });

    emit actionTriggered(item.tip);
}
