#include "taskdialog.h"

TaskDialog::TaskDialog(QWidget *parent)
    : QDialog(parent),
    nameEdit(new QLineEdit(this)),
    descriptionEdit(new QLineEdit(this)),
    startDateEdit(createDateEdit(this)),
    endDateEdit(createDateEdit(this))
{
    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("&Name:"), nameEdit);
    formLayout->addRow(tr("&Description:"), descriptionEdit);
    formLayout->addRow(tr("Start &Date:"), startDateEdit);
    formLayout->addRow(tr("End &Date:"), endDateEdit);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TaskDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TaskDialog::reject);

    auto layout = new QVBoxLayout(this);
    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);
}

QDateEdit* TaskDialog::createDateEdit(QWidget *parent)
{
    auto dateEdit = new QDateEdit(QDate::currentDate(), parent);
    dateEdit->setCalendarPopup(true);
    return dateEdit;
}

QString TaskDialog::taskName() const
{
    return nameEdit->text();
}

QString TaskDialog::taskDescription() const
{
    return descriptionEdit->text();
}

QDate TaskDialog::taskStartDate() const
{
    return startDateEdit->date();
}


QDate TaskDialog::taskEndDate() const
{
    return endDateEdit->date();
}

void TaskDialog::setTaskName(const QString &name)
{
    nameEdit->setText(name);
}

void TaskDialog::setTaskDescription(const QString &description)
{
    descriptionEdit->setText(description);
}

void TaskDialog::setTaskStartDate(const QDate &date)
{
    startDateEdit->setDate(date);
}

void TaskDialog::setTaskEndDate(const QDate &date)
{
    endDateEdit->setDate(date);
}
