#include "launcherpage.h"

#include "icons.h"
#include "shell.h"

#include <QAction>
#include <QFileInfo>
#include <QListWidget>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QStandardPaths>
#include <QStringList>

namespace {

// 从条目命令里解析出"可定位的实体文件完整路径"；
// 电源命令 / 锁屏 / shell 虚拟文件夹 / ms-settings 链接等无对应文件时返回空。
QString resolveTargetFile(const QString &command)
{
    QString cmd = command.trimmed();
    const QString lower = cmd.toLower();

    // 关机/注销/重启、锁屏、shell::: 虚拟项：没有可定位的实体文件
    if (lower.startsWith(QStringLiteral("shutdown"))
        || lower.contains(QStringLiteral("shell:::"))
        || lower.contains(QStringLiteral("lockworkstation"))) {
        return QString();
    }

    // 去掉前导 "start "，取第一个 token 作为程序名（忽略其后参数）
    if (lower.startsWith(QStringLiteral("start "))) {
        cmd = cmd.mid(6).trimmed();
    }
    QString prog = cmd.section(QLatin1Char(' '), 0, 0).trimmed();
    prog.remove(QLatin1Char('"'));
    if (prog.isEmpty()) {
        return QString();
    }

    // 1) 本身就是存在的文件路径
    const QFileInfo direct(prog);
    if (direct.isFile()) {
        return direct.absoluteFilePath();
    }
    // 2) 按 PATH 查找可执行文件（自动补 .exe 等 PATHEXT 后缀）
    const QString fromPath = QStandardPaths::findExecutable(prog);
    if (!fromPath.isEmpty()) {
        return fromPath;
    }
    // 3) 系统目录兜底：.msc / .cpl 等非 PATHEXT 文件多在 System32
    QString sysRoot = QString::fromLocal8Bit(qgetenv("SystemRoot"));
    if (sysRoot.isEmpty()) {
        sysRoot = QString::fromLocal8Bit(qgetenv("windir"));
    }
    if (sysRoot.isEmpty()) {
        sysRoot = QStringLiteral("C:\\Windows");
    }
    const QStringList dirs = {
        sysRoot + QStringLiteral("\\System32"),
        sysRoot,
        sysRoot + QStringLiteral("\\SysWOW64"),
    };
    for (const QString &dir : dirs) {
        const QFileInfo fi(dir + QLatin1Char('\\') + prog);
        if (fi.isFile()) {
            return fi.absoluteFilePath();
        }
        if (!prog.contains(QLatin1Char('.'))) {
            const QFileInfo fiExe(dir + QLatin1Char('\\') + prog + QStringLiteral(".exe"));
            if (fiExe.isFile()) {
                return fiExe.absoluteFilePath();
            }
        }
    }
    return QString();
}

} // namespace

LauncherPage::LauncherPage(const QString &title,
                           const QList<ActionItem> &items,
                           QWidget *parent,
                           bool enableFileLocationMenu)
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

    // 右键菜单：打开对应程序文件所在位置（无法定位到文件的条目不弹菜单）
    if (enableFileLocationMenu) {
        list()->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(list(), &QListWidget::customContextMenuRequested, this,
                [this](const QPoint &pos) {
                    QListWidgetItem *row = list()->itemAt(pos);
                    if (!row) {
                        return;
                    }
                    const int index = list()->row(row);
                    if (index < 0 || index >= m_items.size()) {
                        return;
                    }
                    const ActionItem &item = m_items.at(index);
                    const QString file = resolveTargetFile(item.command);
                    if (file.isEmpty()) {
                        return; // 电源命令 / 锁屏 / shell 虚拟项：无对应文件
                    }
                    QMenu menu(this);
                    QAction *locate = menu.addAction(QStringLiteral("打开文件所在的位置"));
                    if (menu.exec(list()->viewport()->mapToGlobal(pos)) == locate) {
                        Shell::revealInExplorer(file);
                        emit actionTriggered(
                            QStringLiteral("已打开「%1」所在文件夹：%2").arg(item.title, file));
                    }
                });
    }
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
