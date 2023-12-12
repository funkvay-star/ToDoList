#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QModelIndex>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    taskListView = new QListView(this);
    // TODO: Set up the model for taskListView

    auto addButton = new QPushButton(tr("Add Task"), this);
    auto editButton = new QPushButton(tr("Edit Task"), this);
    auto deleteButton = new QPushButton(tr("Delete Task"), this);

    // Connect button signals to their slots
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddTask);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::onEditTask);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteTask);

    // Layout setup for central widget
    auto centralWidget = new QWidget(this);
    auto layout = new QVBoxLayout(centralWidget);
    layout->addWidget(taskListView);
    layout->addWidget(addButton);
    layout->addWidget(editButton);
    layout->addWidget(deleteButton);

    // TODO: Add filter interface components to layout

    setCentralWidget(centralWidget);

    // TODO: Initialize Task Controller and connect it with UI components
}

void MainWindow::onAddTask()
{
    // TODO: Open the Add Task Dialog and add a task
}

void MainWindow::onEditTask()
{
    auto currentIndex = taskListView->currentIndex();
    if (!currentIndex.isValid())
    {
        return;
    }

    // Retrieve the unique identifier of the task from the model
    int taskId = currentIndex.data(Qt::UserRole).toInt();

    // TODO: Open the Task Editor Dialog and edit the task with taskId
}

void MainWindow::onDeleteTask()
{
    auto currentIndex = taskListView->currentIndex();
    if (!currentIndex.isValid())
    {
        return;
    }

    // Retrieve the unique identifier of the task from the model
    int taskId = currentIndex.data(Qt::UserRole).toInt();

    // TODO: Confirm deletion then delete the task with taskId from the model
}

MainWindow::~MainWindow()
{
    delete ui;
}
