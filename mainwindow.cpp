#include "mainwindow.h"
#include "constants.h"
#include "datatable.h"
#include "tabletab.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <stdexcept>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString("GUSE v%1").arg(App::version));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(
        this,
        "Open Data File",
        QString(),
        "Data files (*.csv *.txt *.dat);;All files (*)");

    if (path.isEmpty()) {
        return;
    }

    try {
        DataTable data(path.toStdString());
        auto *tab = new TableTab(nullptr, data);
        int index = ui->tabWidget->addTab(tab, QFileInfo(path).fileName());
        ui->tabWidget->setCurrentIndex(index);
    }
    catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

