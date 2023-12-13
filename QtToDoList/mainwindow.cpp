#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "taskdialog.h"
#include "taskmodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      taskModel(new TaskModel(this))
{
    ui->setupUi(this);

    // Create a new toolbar
    QToolBar *toolbar = new QToolBar(this);
    toolbar->setMovable(false); // Make the toolbar fixed in position

    taskTableView = new QTableView(this);
    taskTableView->setModel(taskModel);

    auto addButton = new QPushButton(tr("Add Task"), this);
    auto editButton = new QPushButton(tr("Edit Task"), this);
    auto deleteButton = new QPushButton(tr("Delete Task"), this);
    auto toggleStateButton = new QPushButton(tr("Toggle State"), this);
    auto saveAction = new QAction(tr("Save"), this);

    // Connect button signals to their slots
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddTask);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::onEditTask);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteTask);
    connect(toggleStateButton, &QPushButton::clicked, this, &MainWindow::onToggleTaskState);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveTasks);


    auto centralWidget = new QWidget(this);
    auto layout = new QVBoxLayout(centralWidget);
    layout->addWidget(taskTableView);
    layout->addWidget(addButton);
    layout->addWidget(editButton);
    layout->addWidget(deleteButton);
    layout->addWidget(toggleStateButton);

    toolbar->addAction(saveAction);
    addToolBar(Qt::TopToolBarArea, toolbar);

    setCentralWidget(centralWidget);
    loadTasks();
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

void MainWindow::onToggleTaskState()
{
    QModelIndex currentIndex = taskTableView->currentIndex();
    if (!currentIndex.isValid())
    {
        QMessageBox::warning(this, tr("Select Task"), tr("Please select a task to toggle its state."));
        return;
    }

    int row = currentIndex.row();
    Task currentTask = taskModel->taskAt(row);
    currentTask.isCompleted = !currentTask.isCompleted;

    taskModel->setTaskAt(row, currentTask);
}

void MainWindow::onSaveTasks()
{
    QFile file("tasks.json");
    qDebug("Save");
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("Error"), tr("Unable to open file for writing."));
        return;
    }

    QJsonArray tasksArray;
    for (const Task &task : taskModel->getTasks())
    {
        QJsonObject taskObject;
        taskObject["name"] = task.name;
        taskObject["description"] = task.description;
        taskObject["date"] = task.date.toString(Qt::ISODate);
        taskObject["isCompleted"] = task.isCompleted;
        tasksArray.append(taskObject);
    }

    QJsonDocument doc(tasksArray);
    file.write(doc.toJson());
    file.close();
}

void MainWindow::loadTasks()
{
    QFile file("tasks.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("Error"), tr("Unable to open file for reading."));
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonArray tasksArray = doc.array();

    for (const QJsonValue &value : tasksArray)
    {
        QJsonObject obj = value.toObject();
        Task task;
        task.name = obj["name"].toString();
        task.description = obj["description"].toString();
        task.date = QDate::fromString(obj["date"].toString(), Qt::ISODate);
        task.isCompleted = obj["isCompleted"].toBool();
        taskModel->addTask(task);
    }

    file.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
