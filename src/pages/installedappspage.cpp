#include "installedappspage.h"

#include "icons.h"
#include "shell.h"

#include <QAction>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QListWidget>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QSet>
#include <QStringList>

#include <windows.h>

namespace {

// 已安装应用信息
struct InstalledApp {
    QString name;
    QString exePath;   // 可执行文件完整路径（双击启动用）
    QString version;
    QString uninstall; // 卸载命令（右键"卸载软件"用，注册表 UninstallString，可能为空）
};

// 展开 %SystemRoot% 之类的环境变量（注册表里常出现未展开的写法）
QString expandEnv(const QString &text)
{
    if (!text.contains(QLatin1Char('%'))) {
        return text;
    }
    std::wstring out(MAX_PATH, L'\0');
    const DWORD n = ExpandEnvironmentStringsW(
        reinterpret_cast<LPCWSTR>(text.utf16()), &out[0], MAX_PATH);
    if (n > 0 && n <= MAX_PATH) {
        return QString::fromWCharArray(out.c_str(), n - 1);
    }
    return text;
}

// DisplayIcon 形如 "C:\path\app.exe" / "C:\path\app.exe,0" / 带引号 / 带参数，
// 解析出真实存在的 exe 完整路径；不是 exe 或路径失效则返回空
QString exeFromDisplayIcon(const QString &displayIcon)
{
    QString s = displayIcon.trimmed();
    s.remove(QLatin1Char('"'));

    // 去掉尾部的图标索引 ",N"
    const int comma = s.lastIndexOf(QLatin1Char(','));
    if (comma > 0) {
        const QString tail = s.mid(comma + 1).trimmed();
        bool numeric = false;
        tail.toInt(&numeric);
        if (numeric) {
            s = s.left(comma);
        }
    }

    s = expandEnv(s).trimmed();
    if (!s.endsWith(QStringLiteral(".exe"), Qt::CaseInsensitive)) {
        return QString();
    }
    if (QFileInfo::exists(s)) {
        return s;
    }
    // 处理 "app.exe -arg" 这种带参数的写法
    const QString first = s.section(QLatin1Char(' '), 0, 0);
    if (first.endsWith(QStringLiteral(".exe"), Qt::CaseInsensitive) && QFileInfo::exists(first)) {
        return first;
    }
    return QString();
}

// 过滤补丁、语言包等非应用条目
bool isJunkEntry(const QString &name)
{
    static const QStringList kJunkKeywords = {
        QStringLiteral("Update for Microsoft"),
        QStringLiteral("Security Update"),
        QStringLiteral("Hotfix"),
        QStringLiteral("Language Pack"),
        QStringLiteral("Microsoft Edge Update"),
        QStringLiteral("Windows Installer Patch"),
    };
    for (const QString &kw : kJunkKeywords) {
        if (name.contains(kw, Qt::CaseInsensitive)) {
            return true;
        }
    }
    return name.startsWith(QStringLiteral("KB"), Qt::CaseInsensitive);
}

// 解析注册表 UninstallString 并启动卸载程序。常见形态：
//   "C:\Program Files\Foo\unins000.exe"          带引号
//   "C:\Program Files\Foo\unins000.exe" /SILENT  带引号 + 参数
//   MsiExec.exe /X{GUID}                          不带引号 + 参数
//   C:\Program Files\Foo\uninstall.exe            不带引号却含空格（少见）
// 拆出「程序 + 参数」交给 QProcess 正确加引号，绕开 cmd /c 的引号剥离问题。
bool runUninstallCommand(const QString &cmdLine)
{
    QString s = cmdLine.trimmed();
    if (s.isEmpty()) {
        return false;
    }

    QString program;
    if (s.startsWith(QLatin1Char('"'))) {
        // 带引号：取第一个闭合引号内的内容作为程序
        const int end = s.indexOf(QLatin1Char('"'), 1);
        if (end > 0) {
            program = s.mid(1, end - 1);
            s = s.mid(end + 1).trimmed();
        } else {
            program = s.mid(1);   // 缺闭合引号：整串当程序
            s.clear();
        }
    } else {
        // 不带引号：逐个 token 追加，直到命中真实存在的路径，
        // 以兼容 "C:\Program Files\..." 这种未加引号却含空格的写法。
        const QStringList tokens = s.split(QLatin1Char(' '), Qt::SkipEmptyParts);
        if (tokens.isEmpty()) {
            return false;
        }
        int cut = 1;   // 都不存在时退回「首个 token 当程序，其余当参数」
        for (int n = 1; n <= tokens.size(); ++n) {
            if (QFileInfo::exists(tokens.mid(0, n).join(QLatin1Char(' ')))) {
                cut = n;
                break;
            }
        }
        program = tokens.mid(0, cut).join(QLatin1Char(' '));
        s = tokens.mid(cut).join(QLatin1Char(' '));
    }

    if (program.isEmpty()) {
        return false;
    }
    const QStringList args = s.isEmpty()
        ? QStringList()
        : s.split(QLatin1Char(' '), Qt::SkipEmptyParts);
    return QProcess::startDetached(program, args);
}

} // namespace

InstalledAppsPage::InstalledAppsPage(QWidget *parent)
    : ListPage(QStringLiteral("安装应用"), parent)
{
    setTitle(QStringLiteral("安装应用（待扫描）"));

    // 双击（或回车）启动对应应用
    connect(list(), &QListWidget::itemActivated, this, [this](QListWidgetItem *row) {
        const QString exe = row->data(Qt::UserRole).toString();
        if (exe.isEmpty()) {
            return;
        }
        if (QProcess::startDetached(exe, {})) {
            emit actionTriggered(QStringLiteral("正在启动：%1").arg(row->text()));
        } else {
            QMessageBox::warning(this,
                                 QStringLiteral("启动失败"),
                                 QStringLiteral("无法启动「%1」：\n%2").arg(row->text(), exe));
        }
    });

    // 右键菜单：打开文件所在位置 / 卸载软件
    list()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(list(), &QListWidget::customContextMenuRequested, this,
            [this](const QPoint &pos) {
                QListWidgetItem *row = list()->itemAt(pos);
                if (!row) {
                    return;
                }
                const QString exe = row->data(Qt::UserRole).toString();
                const QString uninstall = row->data(Qt::UserRole + 1).toString();

                QMenu menu(this);
                QAction *locate = menu.addAction(QStringLiteral("打开文件所在的位置"));
                locate->setEnabled(!exe.isEmpty());
                QAction *uninst = menu.addAction(QStringLiteral("卸载软件"));
                uninst->setEnabled(!uninstall.isEmpty());

                QAction *chosen = menu.exec(list()->viewport()->mapToGlobal(pos));
                if (chosen == locate) {
                    Shell::revealInExplorer(exe);
                    emit actionTriggered(
                        QStringLiteral("已打开「%1」所在文件夹：%2").arg(row->text(), exe));
                } else if (chosen == uninst) {
                    if (runUninstallCommand(uninstall)) {
                        emit actionTriggered(
                            QStringLiteral("正在启动「%1」的卸载程序").arg(row->text()));
                    } else {
                        QMessageBox::warning(this,
                                             QStringLiteral("卸载失败"),
                                             QStringLiteral("无法启动「%1」的卸载程序：\n%2")
                                                 .arg(row->text(), uninstall));
                    }
                }
            });
}

void InstalledAppsPage::ensureLoaded()
{
    if (m_loaded) {
        return;
    }
    m_loaded = true;
    scan();
}

void InstalledAppsPage::scan()
{
    // 三个卸载信息根键：64 位、32 位程序、当前用户安装
    const QStringList roots = {
        QStringLiteral("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"),
        QStringLiteral("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"),
        QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"),
    };

    QList<InstalledApp> apps;
    QSet<QString> seen; // 按名称去重（三个根键可能重复登记同一程序）

    for (const QString &root : roots) {
        QSettings reg(root, QSettings::NativeFormat);
        const QStringList groups = reg.childGroups();
        for (const QString &key : groups) {
            reg.beginGroup(key);

            const QString name = reg.value(QStringLiteral("DisplayName")).toString().trimmed();
            const QString icon = reg.value(QStringLiteral("DisplayIcon")).toString().trimmed();
            const QString version = reg.value(QStringLiteral("DisplayVersion")).toString().trimmed();
            const QString uninstall = reg.value(QStringLiteral("UninstallString")).toString().trimmed();
            const bool systemComponent = reg.value(QStringLiteral("SystemComponent"), 0).toInt() == 1;

            reg.endGroup();

            if (name.isEmpty() || icon.isEmpty() || systemComponent || isJunkEntry(name)) {
                continue;
            }
            if (seen.contains(name.toLower())) {
                continue;
            }

            const QString exe = exeFromDisplayIcon(icon);
            if (exe.isEmpty()) {
                continue; // 拿不到可执行路径就无法双击打开，跳过
            }

            seen.insert(name.toLower());
            apps.append({ name, exe, version, uninstall });
        }
    }

    // 按名称排序（中文按拼音/locale 顺序）
    std::sort(apps.begin(), apps.end(),
              [](const InstalledApp &a, const InstalledApp &b) {
                  return a.name.localeAwareCompare(b.name) < 0;
              });

    // 填充列表，图标直接取 exe 内嵌图标
    QFileIconProvider provider;
    const QIcon fallback = QIcon(Icons::emoji(QStringLiteral("📦"), 64));
    for (const InstalledApp &app : apps) {
        QIcon icon = provider.icon(QFileInfo(app.exePath));
        if (icon.isNull()) {
            icon = fallback;
        }
        auto *row = new QListWidgetItem(icon, app.name, list());
        row->setData(Qt::UserRole, app.exePath);
        row->setData(Qt::UserRole + 1, app.uninstall);
        if (app.version.isEmpty()) {
            row->setToolTip(QStringLiteral("%1\n%2\n\n双击打开").arg(app.name, app.exePath));
        } else {
            row->setToolTip(QStringLiteral("%1（v%2）\n%3\n\n双击打开")
                                .arg(app.name, app.version, app.exePath));
        }
    }

    setTitle(QStringLiteral("安装应用（%1）").arg(apps.count()));
    emit actionTriggered(QStringLiteral("已扫描到 %1 个已安装应用，双击打开，右键可定位文件/卸载").arg(apps.count()));
}
