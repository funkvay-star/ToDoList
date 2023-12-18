#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QModelIndex>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMainWindow>
#include <QTableView>
#include <QToolBar>

#include "taskmodel.h"
#include "filterdialog.h"
#include "taskdialog.h"
#include "customtable.h"
#include "taskinfodialog.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

enum GridPosition
{
    FirstRow = 0,
    SecondRow = 1,
    FirstColumn = 0,
    SecondColumn = 1
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddTask();
    void onEditTask();
    void onDeleteTask();
    void onToggleTaskState();
    void onSaveTasks();
    void onFilterTasks();
    void resetTaskModelFilters();

public slots:
    void showTaskInfo(const QModelIndex &index);

private:
    QAction* createAction(const QString &text, void (MainWindow::*slotFunction)());
    QPushButton* createButton(const QString &text, void (MainWindow::*slotFunction)());
    bool setupTaskFromDialog(Task &task, TaskDialog &dialog);
    void loadTasks();
    bool validateTaskInput(const QString &name, const QDate &startDate, const QDate &endDate);
    void refreshView();

    Ui::MainWindow *ui;
    CustomTableView *taskTableView;
    TaskModel *taskModel;
};

#endif // MAINWINDOW_H
