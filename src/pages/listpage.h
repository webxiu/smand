#pragma once

#include <QWidget>

class QLabel;
class QListWidget;

// 带标题 + 列表的通用内容页基类：
// 同一份数据可在"宫格"和"列表"两种视图间切换
class ListPage : public QWidget
{
    Q_OBJECT

public:
    enum class ViewMode { Grid, List };

    explicit ListPage(const QString &title, QWidget *parent = nullptr);

    void setViewMode(ViewMode mode);
    ViewMode viewMode() const { return m_mode; }

    void setTitle(const QString &title);
    int itemCount() const;

signals:
    // 通知主窗口在状态栏显示执行反馈
    void actionTriggered(const QString &message);

protected:
    // 子类用它填充条目（图标 + 文字 + 悬浮提示）
    QListWidget *list() const { return m_list; }

private:
    void applyViewStyle();

    QLabel *m_titleLabel = nullptr;
    QListWidget *m_list = nullptr;
    ViewMode m_mode = ViewMode::Grid;
};
