#pragma once

#include <QString>

namespace Shell {

// 在 Windows 资源管理器中打开目标文件所在文件夹并高亮选中它。
// 文件不存在或路径为空时返回 false（不发起任何进程）。
bool revealInExplorer(const QString &filePath);

} // namespace Shell
