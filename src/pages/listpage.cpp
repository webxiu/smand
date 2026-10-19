#include "listpage.h"

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>

ListPage::ListPage(const QString &title, QWidget *parent)
    : QWidget(parent)
{
    m_titleLabel = new QLabel(title, this);
    m_titleLabel->setObjectName(QStringLiteral("pageTitle"));

    m_list = new QListWidget(this);
    m_list->setObjectName(QStringLiteral("itemList"));
    m_list->setFocusPolicy(Qt::NoFocus);
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setSelectionRectVisible(false);
    m_list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_list->setUniformItemSizes(false);
    m_list->setCursor(Qt::PointingHandCursor);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 18, 24, 12);
    layout->setSpacing(10);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_list, 1);

    applyViewStyle();
}

void ListPage::setViewMode(ViewMode mode)
{
    if (m_mode == mode) {
        return;
    }
    m_mode = mode;
    applyViewStyle();
}

void ListPage::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
}

int ListPage::itemCount() const
{
    return m_list->count();
}

void ListPage::applyViewStyle()
{
    if (m_mode == ViewMode::Grid) {
        // 宫格：大图标在上，文字换行在下
        m_list->setViewMode(QListView::IconMode);
        m_list->setResizeMode(QListView::Adjust);
        m_list->setMovement(QListView::Static);
        m_list->setWrapping(true);
        m_list->setWordWrap(true);
        m_list->setGridSize(QSize(150, 110));
        m_list->setIconSize(QSize(44, 44));
        m_list->setSpacing(8);
    } else {
        // 列表：小图标在左，单行文字
        m_list->setViewMode(QListView::ListMode);
        m_list->setMovement(QListView::Static);
        m_list->setWrapping(false);
        m_list->setWordWrap(false);
        m_list->setGridSize(QSize());
        m_list->setIconSize(QSize(32, 32));
        m_list->setSpacing(6);
    }
}
