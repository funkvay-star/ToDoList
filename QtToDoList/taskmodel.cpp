#include "taskmodel.h"
#include "QDebug"

bool Task::operator==(const Task& other) const
{
    return name == other.name &&
           description == other.description &&
           startDate == other.startDate &&
           endDate == other.endDate &&
           isCompleted == other.isCompleted;
}

TaskModel::TaskModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int TaskModel::rowCount([[maybe_unused]] const QModelIndex &) const
{
    if (filterApplied)
    {
        return filteredTasks.count();
    }

    return tasks.count();
}

int TaskModel::columnCount([[maybe_unused]] const QModelIndex &) const
{
    return ColumnCount;
}

QString TaskModel::insertLineBreaks(const QString &original, int lineLength) const
{
    QString result;
    int currentPosition = 0;

    while (currentPosition < original.length())
    {
        int lineEnd = std::min(currentPosition + lineLength, original.length());
        int breakPosition = findBreakPosition(original, currentPosition, lineEnd);

        result += original.mid(currentPosition, breakPosition - currentPosition).trimmed() + "\n";
        currentPosition = breakPosition + 1;
    }

    return result.trimmed();
}

int TaskModel::findBreakPosition(const QString &text, int start, int end) const
{
    int spacePosition = text.lastIndexOf(' ', end);
    return (spacePosition > start) ? spacePosition : end;
}


QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    const QVector<Task>& taskSource = filterApplied ? filteredTasks : tasks;
    const Task& task = taskSource.at(index.row());

    switch (index.column())
    {
        case NameColumn:
            return insertLineBreaks(task.name, 75);
        case DescriptionColumn:
            return insertLineBreaks(task.description, 75);
        case StartDateColumn:
            return task.startDate.toString(Qt::ISODate);
        case EndDateColumn:
            return task.endDate.toString(Qt::ISODate);
        case IsCompletedColumn:
            return task.isCompleted ? "Completed" : "In Progress";
        default:
            return QVariant();
    }
}

QVariant TaskModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    {
        return QVariant();
    }

    switch (section)
    {
        case NameColumn:
            return QStringLiteral("Name");
        case DescriptionColumn:
            return QStringLiteral("Description");
        case StartDateColumn:
            return QStringLiteral("Start Date");
        case EndDateColumn:
            return QStringLiteral("End Date");
        case IsCompletedColumn:
            return QStringLiteral("Status");
        default:
            return QVariant();
    }
}

bool TaskModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
    {
        return false;
    }

    Task &task = tasks[index.row()];
    switch (index.column())
    {
        case NameColumn:
            task.name = value.toString();
            break;
        case DescriptionColumn:
            task.description = value.toString();
            break;
        case StartDateColumn:
            task.startDate = QDate::fromString(value.toString(), Qt::ISODate);
            break;
        case EndDateColumn:
            task.endDate = QDate::fromString(value.toString(), Qt::ISODate);
            break;
        case IsCompletedColumn:
            task.isCompleted = value.toBool();
            break;
        default:
            return false;
    }

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags TaskModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    return QAbstractTableModel::flags(index);
}

Task TaskModel::taskAt(int row) const
{
    if (row >= 0 && row < tasks.size())
    {
        return tasks.at(row);
    }

    return Task();
}

void TaskModel::setTaskAt(int row, const Task &task)
{
    if (row < 0 || row >= tasks.size())
    {
        return;
    }

    tasks[row] = task;

    // Notify the view that data has changed for this row
    QModelIndex startIndex = index(row, 0);
    QModelIndex endIndex = index(row, ColumnCount - 1);
    emit dataChanged(startIndex, endIndex);

    // Update filteredTasks if filter is applied
    if (filterApplied)
    {
        int filteredRow = filteredTasks.indexOf(task);
        if (filteredRow != -1)
        {
            filteredTasks[filteredRow] = task;
            QModelIndex startFilteredIndex = index(filteredRow, 0);
            QModelIndex endFilteredIndex = index(filteredRow, ColumnCount - 1);
            emit dataChanged(startFilteredIndex, endFilteredIndex);
        }
    }
}


bool TaskModel::addTask(const Task &task)
{
    const int index = tasks.size();
    beginInsertRows(QModelIndex(), index, index);
    tasks.append(task);
    endInsertRows();
    return true;
}

bool TaskModel::removeTask(int row)
{
    if (row < 0 || row >= tasks.size())
    {
        return false;
    }

    // Remove from the main list
    beginRemoveRows(QModelIndex(), row, row);
    Task removedTask = tasks.takeAt(row);
    endRemoveRows();

    // Remove from filtered list if filter is applied
    if (filterApplied)
    {
        int filteredRow = filteredTasks.indexOf(removedTask);
        if (filteredRow != -1)
        {
            beginRemoveRows(QModelIndex(), filteredRow, filteredRow);
            filteredTasks.removeAt(filteredRow);
            endRemoveRows();
        }
    }

    return true;
}

QVector<Task> TaskModel::getTasks() const
{
    return tasks;
}

void TaskModel::setFilterCriteria(const QString &name, const QString &description, const QDate &startDate, const QDate &endDate, bool status)
{
    filterName = name;
    filterDescription = description;
    filterStartDate = startDate;
    filterEndDate = endDate;
    filterStatus = status;
}

void TaskModel::applyFilter()
{
    updateFilteredTasks();
    filterApplied = true;
    emit dataChanged(createIndex(FirstRow, FirstColumn),
                     createIndex(rowCount() - 1, columnCount() - 1));
}

void TaskModel::resetFilter()
{
    filterApplied = false;
    updateFilteredTasks();
    emit dataChanged(createIndex(FirstRow, FirstColumn),
                     createIndex(rowCount() - 1, columnCount() - 1));
}

bool TaskModel::matchesName(const Task& task) const
{
    return filterName.isEmpty() || task.name.contains(filterName, Qt::CaseInsensitive);
}

bool TaskModel::matchesDescription(const Task& task) const
{
    return filterDescription.isEmpty() || task.description.contains(filterDescription, Qt::CaseInsensitive);
}

bool TaskModel::matchesStartDate(const Task& task) const
{
    return !filterStartDate.isValid() || task.startDate >= filterStartDate;
}

bool TaskModel::matchesEndDate(const Task& task) const
{
    return !filterEndDate.isValid() || task.endDate <= filterEndDate;
}

bool TaskModel::matchesStatus(const Task& task) const
{
    return filterStatus == task.isCompleted;
}

int TaskModel::mapToSourceRow(int filteredRow) const
{
    if (!filterApplied)
    {
        return filteredRow;
    }

    if (filteredRow >= 0 && filteredRow < filteredTasks.size())
    {
        const Task& filteredTask = filteredTasks.at(filteredRow);
        return tasks.indexOf(filteredTask);
    }

    return -1;
}

bool TaskModel::isFilterApplied() const
{
    return filterApplied;
}

void TaskModel::updateFilteredTasks()
{
    filteredTasks.clear();
    for (const auto &task : tasks)
    {
        if (matchesName(task) && matchesDescription(task) &&
            matchesStartDate(task) && matchesEndDate(task) &&
            matchesStatus(task))
        {
            filteredTasks.push_back(task);
        }
    }
}

