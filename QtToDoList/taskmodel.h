#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QAbstractTableModel>
#include <QDate>
#include <QVector>

class Task
{
public:
    bool operator==(const Task& other) const
    {
        return name == other.name &&
               description == other.description &&
               startDate == other.startDate &&
               endDate == other.endDate &&
               isCompleted == other.isCompleted;
    }

    QString name;
    QString description;
    QDate startDate;
    QDate endDate;
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
        StartDateColumn,
        EndDateColumn,
        IsCompletedColumn,
        ColumnCount
    };

    TaskModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QString insertLineBreaks(const QString &original, int lineLength) const;
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

    // Additional functionality
    QVector<Task> getTasks() const;
    bool matchesName(const Task& task) const;
    bool matchesDescription(const Task& task) const;
    bool matchesStartDate(const Task& task) const;
    bool matchesEndDate(const Task& task) const;
    bool matchesStatus(const Task& task) const;
    int mapToSourceRow(int filteredRow) const;
    bool isFilterApplied() const;

    // Filtering
    void setFilterCriteria(const QString &name, const QString &description,
                           const QDate &startDate, const QDate &endDate, bool status);
    void applyFilter();
    void resetFilter();

private:
    QVector<Task> tasks;
    QVector<Task> filteredTasks; // To store filtered tasks
    bool filterApplied = false;
    static constexpr int FirstRow = 0;
    static constexpr int FirstColumn = 0;

    // Filter criteria
    QString filterName;
    QString filterDescription;
    QDate filterStartDate;
    QDate filterEndDate;
    bool filterStatus;

    void updateFilteredTasks(); // Internal method to update filteredTasks based on filter criteria
};

#endif // TASKMODEL_H
