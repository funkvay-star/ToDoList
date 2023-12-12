#include "taskdialog.h"

TaskDialog::TaskDialog(QWidget *parent)
    : QDialog(parent)
{
    nameEdit = new QLineEdit(this);
    descriptionEdit = new QLineEdit(this);
    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setCalendarPopup(true);

    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("&Name:"), nameEdit);
    formLayout->addRow(tr("&Description:"), descriptionEdit);
    formLayout->addRow(tr("&Date:"), dateEdit);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TaskDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TaskDialog::reject);

    auto layout = new QVBoxLayout(this);
    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);
}

QString TaskDialog::taskName() const
{
    return nameEdit->text();
}

QString TaskDialog::taskDescription() const
{
    return descriptionEdit->text();
}

QDate TaskDialog::taskDate() const
{
    return dateEdit->date();
}
