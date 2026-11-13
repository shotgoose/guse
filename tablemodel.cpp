#include "tablemodel.h"

TableModel::TableModel(QObject *parent, const DataTable &data)
    : QAbstractTableModel{parent}, m_data(data) {}

int TableModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return m_data.rowCount();
}

int TableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return m_data.columnCount();
}

QVariant TableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        return m_data.entry(index.row(), index.column());
    }

    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        auto headers = m_data.headers();
        if (section >= 0 && section < headers.size()) {
            return QString::fromStdString(headers[section]);
        }
    } else if (orientation == Qt::Vertical) {
        return section + 1;
    }

    return QVariant();
}

void TableModel::setTableData(const DataTable &data) {
    beginResetModel();
    m_data = data;
    endResetModel();
}