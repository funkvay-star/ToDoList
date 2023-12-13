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
    auto filterAction = new QAction(tr("Filter Tasks"), this);

    // Connect button signals to their slots
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddTask);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::onEditTask);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteTask);
    connect(toggleStateButton, &QPushButton::clicked, this, &MainWindow::onToggleTaskState);

    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveTasks);
    connect(filterAction, &QAction::triggered, this, &MainWindow::onFilterTasks);

    auto centralWidget = new QWidget(this);
    auto layout = new QVBoxLayout(centralWidget);
    layout->addWidget(taskTableView);
    layout->addWidget(addButton);
    layout->addWidget(editButton);
    layout->addWidget(deleteButton);
    layout->addWidget(toggleStateButton);

    toolbar->addAction(saveAction);
    toolbar->addAction(filterAction);
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

        if(!validateTaskInput(dialog.taskName(), dialog.taskStartDate(), dialog.taskEndDate()))
        {
            return;
        }

        newTask.name = dialog.taskName();
        newTask.description = dialog.taskDescription();
        newTask.startDate = dialog.taskStartDate();
        newTask.endDate = dialog.taskEndDate();
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
    dialog.setTaskStartDate(currentTask.startDate);
    dialog.setTaskEndDate(currentTask.endDate);

    if (dialog.exec() == QDialog::Accepted)
    {
        Task updatedTask;

        if(!validateTaskInput(dialog.taskName(), dialog.taskStartDate(), dialog.taskEndDate()))
        {
            return;
        }

        updatedTask.name = dialog.taskName();
        updatedTask.description = dialog.taskDescription();
        updatedTask.startDate = dialog.taskStartDate();
        updatedTask.endDate = dialog.taskEndDate();
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
        taskObject["startDate"] = task.startDate.toString(Qt::ISODate);
        taskObject["endDate"] = task.endDate.toString(Qt::ISODate);
        taskObject["isCompleted"] = task.isCompleted;
        tasksArray.append(taskObject);
    }

    QJsonDocument doc(tasksArray);
    file.write(doc.toJson());
    file.close();
}

void MainWindow::onFilterTasks()
{
    FilterDialog filterDialog(this);

    // Connect the reset signal to a slot
    connect(&filterDialog, &FilterDialog::filtersReset, this, &MainWindow::resetTaskModelFilters);

    if (filterDialog.exec() == QDialog::Accepted) {
        taskModel->setFilterCriteria(filterDialog.getFilterName(), filterDialog.getFilterDescription(), filterDialog.getFilterStartDate(), filterDialog.getFilterEndDate(), filterDialog.getFilterStatus());
        taskModel->applyFilter();
    }
}

void MainWindow::resetTaskModelFilters()
{
    taskModel->resetFilter();
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
        task.startDate = QDate::fromString(obj["startDate"].toString(), Qt::ISODate);
        task.endDate = QDate::fromString(obj["endDate"].toString(), Qt::ISODate);
        task.isCompleted = obj["isCompleted"].toBool();
        taskModel->addTask(task);
    }

    file.close();
}

bool MainWindow::validateTaskInput(const QString &name, const QDate &startDate, const QDate &endDate)
{
    if (name.isEmpty())
    {
        QMessageBox::warning(this, tr("Task Name"), tr("Please provide a name for the task."));
        return false;
    }

    if (endDate < startDate)
    {
        QMessageBox::warning(this, tr("Invalid Dates"), tr("The end date cannot be earlier than the start date."));
        return false;
    }

    return true;
}

MainWindow::~MainWindow()
{
    delete ui;
}
