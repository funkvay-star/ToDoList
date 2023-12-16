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

    QDateEdit* createDateEdit(QWidget *parent);
    QString taskName() const;
    QString taskDescription() const;
    QDate taskStartDate() const;
    QDate taskEndDate() const;
    void setTaskName(const QString &name);
    void setTaskDescription(const QString &description);
    void setTaskStartDate(const QDate &date);
    void setTaskEndDate(const QDate &date);

private:
    QLineEdit *nameEdit;
    QLineEdit *descriptionEdit;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
};

#endif // TASKDIALOG_H
