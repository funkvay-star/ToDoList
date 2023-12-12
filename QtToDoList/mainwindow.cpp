#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QModelIndex>

#include "taskdialog.h"
#include "taskmodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      taskModel(new TaskModel(this))
{
    ui->setupUi(this);

    taskTableView = new QTableView(this);
    taskTableView->setModel(taskModel);

    auto addButton = new QPushButton(tr("Add Task"), this);
    auto editButton = new QPushButton(tr("Edit Task"), this);
    auto deleteButton = new QPushButton(tr("Delete Task"), this);

    // Connect button signals to their slots
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddTask);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::onEditTask);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteTask);

    auto centralWidget = new QWidget(this);
    auto layout = new QVBoxLayout(centralWidget);
    layout->addWidget(taskTableView);
    layout->addWidget(addButton);
    layout->addWidget(editButton);
    layout->addWidget(deleteButton);

    setCentralWidget(centralWidget);
}

void MainWindow::onAddTask()
{
    TaskDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Task newTask;
        newTask.name = dialog.taskName();
        newTask.description = dialog.taskDescription();
        newTask.date = dialog.taskDate();
        newTask.isCompleted = false;

        taskModel->addTask(newTask);
    }
}

void MainWindow::onEditTask()
{
    QModelIndex currentIndex = taskTableView->currentIndex();
    if (!currentIndex.isValid())
    {
        QMessageBox::warning(this, tr("Select Task"), tr("Please select a task to edit."));
        return;
    }
    // TODO: Implement logic to edit a task
}

void MainWindow::onDeleteTask()
{
    QModelIndex currentIndex = taskTableView->currentIndex();
    if (!currentIndex.isValid())
    {
        QMessageBox::warning(this, tr("Select Task"), tr("Please select a task to delete."));
        return;
    }
    // TODO: Implement logic to delete a task
}

MainWindow::~MainWindow()
{
    delete ui;
}
