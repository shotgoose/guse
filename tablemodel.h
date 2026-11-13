#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include "datatable.h"

class TableModel : public QAbstractTableModel
{
public:
    explicit TableModel(QObject *parent = nullptr, const DataTable &data = DataTable());

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // 2. Optional: Add header labels for columns
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void setTableData(const DataTable &data);

private:
    DataTable m_data;
};

#endif // TABLEMODEL_H
