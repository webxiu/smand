#include "icons.h"

#include <QFont>
#include <QPainter>
#include <QPixmap>

namespace Icons {

QPixmap emoji(const QString &text, int size)
{
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QFont font(QStringLiteral("Segoe UI Emoji"));
    font.setPixelSize(qRound(size * 0.75));
    painter.setFont(font);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, text);
    return pixmap;
}

} // namespace Icons
