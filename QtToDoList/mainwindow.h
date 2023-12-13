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

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

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

private:
    void loadTasks();
    bool validateTaskInput(const QString &name, const QDate &startDate, const QDate &endDate);

    Ui::MainWindow *ui;
    QTableView *taskTableView;
    TaskModel *taskModel;
};

#endif // MAINWINDOW_H
