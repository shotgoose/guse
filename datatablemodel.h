#ifndef DATATABLEMODEL_H
#define DATATABLEMODEL_H

#include "datatable.h"
#include <QAbstractTableModel>

class DataTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DataTableModel(const DataTable &table, QObject *parent = nullptr)
        : QAbstractTableModel(parent), m_table(table) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
private:
    DataTable m_table;
};

#endif // DATATABLEMODEL_H
