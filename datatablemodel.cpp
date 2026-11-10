#include "datatablemodel.h"

int DataTableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_table.rowCount();
}

int DataTableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_table.columnCount();
}

QVariant DataTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    int r = index.row(), c = index.column();
    if (m_table.isNumeric(c))
        return m_table.column(c)[r];
    return QString::fromStdString(m_table.textColumn(c)[r]);
}

QVariant DataTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};
    if (orientation == Qt::Horizontal)
        return QString::fromStdString(m_table.headers()[section]);
    return section + 1;   // row numbers
}