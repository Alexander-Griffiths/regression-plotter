#include "tablemodel.h"

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{

}

void TableModel::populateData(const QVector<double> x_data, const QVector<double> y_data)
{
    x.clear();
    y.clear();
    x = x_data;
    y = y_data;
}

int TableModel::rowCount(const QModelIndex &) const
{
    return x.size();
}

int TableModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
            return QVariant();
        }
        if (index.column() == 0) {
            return x.at(index.row());
        } else if (index.column() == 1) {
            return y.at(index.row());
        }
        return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return QString("X");
        } else if (section == 1) {
            return QString("Y");
        }
    }
    return QVariant();
}

void TableModel::clear()
{
    this->beginResetModel();
    this->endResetModel();
}
