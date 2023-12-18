#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QTableView>
#include <QResizeEvent>

class CustomTableView : public QTableView
{
    Q_OBJECT

public:
    explicit CustomTableView(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void adjustRowHeight();
};

#endif // CUSTOMTABLEVIEW_H
