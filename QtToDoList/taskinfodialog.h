#ifndef TASKINFODIALOG_H
#define TASKINFODIALOG_H

#include <QDialog>
#include <QPushButton>

#include "taskmodel.h"

class TaskInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TaskInfoDialog(const Task& task, QWidget *parent = nullptr);
};

#endif // TASKINFODIALOG_H
