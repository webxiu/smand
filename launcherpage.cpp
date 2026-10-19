#include "launcherpage.h"

#include "icons.h"

#include <QListWidget>
#include <QMessageBox>
#include <QProcess>

LauncherPage::LauncherPage(const QString &title,
                           const QList<ActionItem> &items,
                           QWidget *parent)
    : ListPage(title, parent)
    , m_items(items)
{
    for (const ActionItem &item : m_items) {
        auto *row = new QListWidgetItem(Icons::emoji(item.emoji, 64), item.title, list());
        row->setToolTip(item.tip);
    }

    // 单击条目即执行（与旧版按钮行为一致）
    connect(list(), &QListWidget::itemClicked, this, [this](QListWidgetItem *row) {
        const int index = list()->row(row);
        if (index >= 0 && index < m_items.size()) {
            executeItem(m_items.at(index));
        }
    });
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
