#ifndef TABLETAB_H
#define TABLETAB_H
#include "datatable.h"
#include <QVariant>
#include <QWidget>
#include <QTableWidget>

class TableTab : public QWidget
{
    Q_OBJECT
public:
    explicit TableTab(QWidget *parent = nullptr, const DataTable &data = DataTable());
    void SetData(const DataTable &data);
private:
    QTableView *m_table;

signals:
};

#endif // TABLETAB_H
