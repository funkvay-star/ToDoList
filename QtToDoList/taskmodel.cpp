#include "taskmodel.h"

TaskModel::TaskModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int TaskModel::rowCount(const QModelIndex & /* parent not needed */) const
{
    return tasks.count();
}

int TaskModel::columnCount(const QModelIndex & /* parent not needed */) const
{
    return ColumnCount;
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    const Task &task = tasks.at(index.row());
    switch (index.column())
    {
        case NameColumn:
            return task.name;
        case DescriptionColumn:
            return task.description;
        case DateColumn:
            return task.date.toString(Qt::ISODate); // Change format as needed
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
        case DateColumn:
            return QStringLiteral("Date");
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
        case DateColumn:
            task.date = QDate::fromString(value.toString(), Qt::ISODate); // Change format as needed
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

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
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

    beginRemoveRows(QModelIndex(), row, row);
    tasks.removeAt(row);
    endRemoveRows();
    return true;
}
