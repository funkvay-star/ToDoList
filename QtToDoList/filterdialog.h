// FilterDialog.h
#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    FilterDialog(QWidget *parent = nullptr);

    QString getFilterName() const;
    QString getFilterDescription() const;
    QDate getFilterStartDate() const;
    QDate getFilterEndDate() const;
    bool getFilterStatus() const; // True for completed, false for in progress

    void resetFilters();

private:
    QLineEdit *nameEdit;
    QLineEdit *descriptionEdit;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QCheckBox *statusCheckbox;
};

#endif // FILTERDIALOG_H
