#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QAbstractTableModel>
#include <QDate>
#include <QVector>

struct Task {
    QString name;
    QString description;
    QDate date;
    bool isCompleted;
};

class TaskModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Columns
    {
        NameColumn,
        DescriptionColumn,
        DateColumn,
        IsCompletedColumn,
        ColumnCount
    };

    TaskModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Task taskAt(int row) const;
    void setTaskAt(int row, const Task &task);

    // Add and Remove data:
    bool addTask(const Task &task);
    bool removeTask(int row);

private:
    QVector<Task> tasks;
};

#endif // TASKMODEL_H
