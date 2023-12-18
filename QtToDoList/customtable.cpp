#include "customtable.h"

CustomTableView::CustomTableView(QWidget *parent)
    : QTableView(parent)
{
}

void CustomTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    adjustRowHeight();
}

void CustomTableView::adjustRowHeight()
{
    for (int row = 0; row < model()->rowCount(); ++row)
    {
        resizeRowToContents(row);
    }
}
