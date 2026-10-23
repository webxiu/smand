#include "shell.h"

#include <QDir>
#include <QFileInfo>
#include <QProcess>

namespace Shell {

bool revealInExplorer(const QString &filePath)
{
    if (filePath.isEmpty()) {
        return false;
    }
    const QFileInfo info(filePath);
    if (!info.exists()) {
        return false;
    }
    // explorer /select,<路径>：打开所在文件夹并选中该文件。
    // 用原生反斜杠路径；含空格的路径（如 C:\Program Files\...）由 QProcess 自动加引号。
    const QString arg = QStringLiteral("/select,") + QDir::toNativeSeparators(info.absoluteFilePath());
    return QProcess::startDetached(QStringLiteral("explorer"), { arg });
}

} // namespace Shell
