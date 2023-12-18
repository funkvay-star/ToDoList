#include "taskinfodialog.h"
#include <QVBoxLayout>
#include <QLabel>

TaskInfoDialog::TaskInfoDialog(const Task& task, QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *nameLabel = new QLabel(tr("Name: %1").arg(task.name));
    QLabel *descriptionLabel = new QLabel(tr("Description: %1").arg(task.description));
    descriptionLabel->setWordWrap(true);  // To handle long texts

    QLabel *startDateLabel = new QLabel(tr("Start Date: %1").arg(task.startDate.toString(Qt::ISODate)));
    QLabel *endDateLabel = new QLabel(tr("End Date: %1").arg(task.endDate.toString(Qt::ISODate)));
    QLabel *statusLabel = new QLabel(tr("Status: %1").arg(task.isCompleted ? "Completed" : "In Progress"));

    layout->addWidget(nameLabel);
    layout->addWidget(descriptionLabel);
    layout->addWidget(startDateLabel);
    layout->addWidget(endDateLabel);
    layout->addWidget(statusLabel);

    QPushButton *closeButton = new QPushButton(tr("Close"));
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeButton);
}
