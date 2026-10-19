#pragma once

class QPixmap;
class QString;

// 图标工具
namespace Icons {

// 把 emoji 文字渲染成 QPixmap，用作菜单/按钮图标
QPixmap emoji(const QString &text, int size);

} // namespace Icons
