#include "tabletab.h"
#include <QVBoxLayout>
#include <qheaderview.h>
#include <qscrollbar.h>

TableTab::TableTab(QWidget *parent, const DataTable &data)
    : QWidget{parent}
{
    auto *layout = new QVBoxLayout(this);
    m_table = new QTableWidget(this);
    layout->addWidget(m_table);

    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);   // read-only
    m_table->setAlternatingRowColors(true);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->horizontalHeader()->setStretchLastSection(true);

    SetData(data);
}

void TableTab::SetData(const DataTable &data) {
    int cols = data.columnCount();
    int rows = data.rowCount();

    m_table->clear();
    m_table->setColumnCount(cols);
    m_table->setRowCount(rows);
    m_table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_table->verticalScrollBar()->setSingleStep(10);

    QStringList labels;
    for (const auto &h : data.headers()) {
        labels << QString::fromStdString(h);
    }
    m_table->setHorizontalHeaderLabels(labels);

    for (int c = 0; c < cols; ++c) {
        if (data.isNumeric(c)) {
            const std::vector<double> col = data.column(c);
            for (int r = 0; r < rows; ++r) {
                m_table->setItem(r, c, new QTableWidgetItem(QString::number(col[r])));
            }
        }
        else {
            const std::vector<std::string> col = data.textColumn(c);
            for (int r = 0; r < rows; ++r) {
                m_table->setItem(r, c, new QTableWidgetItem(QString::fromStdString(col[r])));
            }
        }
    }
}
