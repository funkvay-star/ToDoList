#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QTableView>
#include <QResizeEvent>

class CustomTableView : public QTableView
{
    Q_OBJECT

public:
    explicit CustomTableView(QWidget *parent = nullptr);

signals:
    void taskDoubleClicked(const QModelIndex &index);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    void adjustRowHeight();

};

#endif // CUSTOMTABLEVIEW_H
