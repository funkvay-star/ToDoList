#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      taskModel(new TaskModel(this))
{
    ui->setupUi(this);

    // Create a new toolbar
    QToolBar *toolbar = new QToolBar(this);
    toolbar->setMovable(false);

    taskTableView = new QTableView(this);
    taskTableView->setModel(taskModel);

    auto addButton = createButton("Add Task", &MainWindow::onAddTask);
    auto editButton = createButton("Edit Task", &MainWindow::onEditTask);
    auto deleteButton = createButton("Delete Task", &MainWindow::onDeleteTask);
    auto toggleStateButton = createButton("Toggle State", &MainWindow::onToggleTaskState);

    auto saveAction = createAction("Save", &MainWindow::onSaveTasks);
    auto filterAction = createAction("Filter Tasks", &MainWindow::onFilterTasks);

    // Set up the layout
    auto centralWidget = new QWidget(this);
    auto layout = new QVBoxLayout(centralWidget);
    layout->addWidget(taskTableView);
    layout->addWidget(addButton);
    layout->addWidget(editButton);
    layout->addWidget(deleteButton);
    layout->addWidget(toggleStateButton);

    // Add actions to the toolbar
    toolbar->addAction(saveAction);
    toolbar->addAction(filterAction);
    addToolBar(Qt::TopToolBarArea, toolbar);

    setCentralWidget(centralWidget);

    loadTasks();
}

QAction* MainWindow::createAction(const QString &text, void (MainWindow::*slotFunction)())
{
    QAction *action = new QAction(tr(text.toUtf8().constData()), this);
    connect(action, &QAction::triggered, this, slotFunction);
    return action;
}

QPushButton* MainWindow::createButton(const QString &text, void (MainWindow::*slotFunction)())
{
    QPushButton *button = new QPushButton(tr(text.toUtf8().constData()), this);
    connect(button, &QPushButton::clicked, this, slotFunction);
    return button;
}

bool MainWindow::setupTaskFromDialog(Task &task, TaskDialog &dialog)
{
    if (!validateTaskInput(dialog.taskName(), dialog.taskStartDate(), dialog.taskEndDate()))
    {
        return false;
    }

    task.name = dialog.taskName();
    task.description = dialog.taskDescription();
    task.startDate = dialog.taskStartDate();
    task.endDate = dialog.taskEndDate();

    // Note: The isCompleted attribute should be set outside as it depends on the context (add or edit)
    return true;
}


void MainWindow::onAddTask()
{
    TaskDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Task newTask;
        newTask.isCompleted = false;

        if (setupTaskFromDialog(newTask, dialog))
        {
            taskModel->addTask(newTask);
        }
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
        if (setupTaskFromDialog(currentTask, dialog))
        {
            taskModel->setTaskAt(currentIndex.row(), currentTask);
        }
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
        QMessageBox::critical(this, tr("Error"), tr("Unable to open file for writing: %1").arg(file.errorString()));
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
    if (file.write(doc.toJson()) == -1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Failed to write to file: %1").arg(file.errorString()));
    }

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

    if (!file.exists() || file.size() == 0) // Check if the file doesn't exist or is empty
    {
        if (file.open(QIODevice::WriteOnly))
        {
            file.close();
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), tr("Unable to create file: %1").arg(file.errorString()));
        }

        return;
    }

    // Now, try to open the file for reading
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, tr("Error"), tr("Unable to open file for reading: %1").arg(file.errorString()));
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));

    if (doc.isNull())
    {
        QMessageBox::critical(this, tr("Error"), tr("Failed to parse the file."));
        file.close();
        return;
    }

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
