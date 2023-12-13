#include "filterdialog.h"

FilterDialog::FilterDialog(QWidget *parent) : QDialog(parent)
{
    nameEdit = new QLineEdit(this);
    descriptionEdit = new QLineEdit(this);
    startDateEdit = new QDateEdit(this);
    endDateEdit = new QDateEdit(this);
    statusCheckbox = new QCheckBox(tr("Completed"), this);

    startDateEdit->setCalendarPopup(true);
    endDateEdit->setCalendarPopup(true);

    auto *layout = new QFormLayout(this);
    layout->addRow(tr("Name:"), nameEdit);
    layout->addRow(tr("Description:"), descriptionEdit);
    layout->addRow(tr("Start Date:"), startDateEdit);
    layout->addRow(tr("End Date:"), endDateEdit);
    layout->addRow(statusCheckbox);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Reset, this);
    layout->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttons->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, &FilterDialog::resetFilters);
}

QString FilterDialog::getFilterName() const
{
    return nameEdit->text();
}

QString FilterDialog::getFilterDescription() const
{
    return descriptionEdit->text();
}

QDate FilterDialog::getFilterStartDate() const
{
    return startDateEdit->date();
}

QDate FilterDialog::getFilterEndDate() const
{
    return endDateEdit->date();
}

bool FilterDialog::getFilterStatus() const
{
    return statusCheckbox->isChecked();
}

void FilterDialog::resetFilters()
{
    emit filtersReset();
}
