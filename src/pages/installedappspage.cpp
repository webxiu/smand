#include "installedappspage.h"

#include "icons.h"

#include <QFileIconProvider>
#include <QFileInfo>
#include <QListWidget>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QSet>

#include <windows.h>

namespace {

// 已安装应用信息
struct InstalledApp {
    QString name;
    QString exePath;   // 可执行文件完整路径（双击启动用）
    QString version;
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
            apps.append({ name, exe, version });
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
        if (app.version.isEmpty()) {
            row->setToolTip(QStringLiteral("%1\n%2\n\n双击打开").arg(app.name, app.exePath));
        } else {
            row->setToolTip(QStringLiteral("%1（v%2）\n%3\n\n双击打开")
                                .arg(app.name, app.version, app.exePath));
        }
    }

    setTitle(QStringLiteral("安装应用（%1）").arg(apps.count()));
    emit actionTriggered(QStringLiteral("已扫描到 %1 个已安装应用，双击即可打开").arg(apps.count()));
}
