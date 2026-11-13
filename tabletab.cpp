#include "tabletab.h"
#include "tablemodel.h"
#include <QVBoxLayout>
#include <QStyledItemDelegate>
#include <qheaderview.h>
#include <qscrollbar.h>

TableTab::TableTab(QWidget *parent, const DataTable &data)
    : QWidget{parent}
{
    auto *layout = new QVBoxLayout(this);
    m_table = new QTableView(this);
    auto model = new TableModel(this, data);

    m_table->setModel(model);
    layout->addWidget(m_table);

    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);   // read-only
    m_table->setAlternatingRowColors(true);
    m_table->setItemDelegate(new QStyledItemDelegate(m_table));
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->horizontalHeader()->setStretchLastSection(true);
}
