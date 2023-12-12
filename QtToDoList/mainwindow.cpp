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
        if(dialog.taskName().isEmpty())
        {
            QMessageBox::warning(this, tr("Task Name"), tr("Please provide a name for the task"));
            return;
        }

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

    Task currentTask = taskModel->taskAt(currentIndex.row());

    TaskDialog dialog(this);
    dialog.setTaskName(currentTask.name);
    dialog.setTaskDescription(currentTask.description);
    dialog.setTaskDate(currentTask.date);

    if (dialog.exec() == QDialog::Accepted)
    {
        Task updatedTask;

        if(dialog.taskName().isEmpty())
        {
            QMessageBox::warning(this, tr("Task Name"), tr("Please provide a name for the task"));
            return;
        }

        updatedTask.name = dialog.taskName();
        updatedTask.description = dialog.taskDescription();
        updatedTask.date = dialog.taskDate();
        updatedTask.isCompleted = currentTask.isCompleted;

        taskModel->setTaskAt(currentIndex.row(), updatedTask);
    }
}


void MainWindow::onDeleteTask()
{
    QModelIndex currentIndex = taskTableView->currentIndex();
    if (!currentIndex.isValid())
    {
        QMessageBox::warning(this, tr("Select Task"), tr("Please select a task to delete."));
        return;
    }

    auto response = QMessageBox::question(this, tr("Delete Task"),
                                          tr("Are you sure you want to delete this task?"),
                                          QMessageBox::Yes | QMessageBox::No);

    if (response == QMessageBox::Yes)
    {
        int row = currentIndex.row();
        taskModel->removeTask(row);
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
