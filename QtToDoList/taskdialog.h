// TaskDialog.h

#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QDialogButtonBox>

class TaskDialog : public QDialog
{
    Q_OBJECT

public:
    TaskDialog(QWidget *parent = nullptr);

    QString taskName() const;
    QString taskDescription() const;
    QDate taskDate() const;

private:
    QLineEdit *nameEdit;
    QLineEdit *descriptionEdit;
    QDateEdit *dateEdit;
};

#endif // TASKDIALOG_H
