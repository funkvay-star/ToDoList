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

void CustomTableView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex index = this->indexAt(event->pos());
    if (index.isValid())
    {
        emit taskDoubleClicked(index);
    }

    QTableView::mouseDoubleClickEvent(event);
}

void CustomTableView::adjustRowHeight()
{
    for (int row = 0; row < model()->rowCount(); ++row)
    {
        resizeRowToContents(row);
    }
}
