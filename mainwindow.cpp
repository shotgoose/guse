#include "mainwindow.h"
#include "constants.h"
#include "datatable.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

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

    // QFile file(path);
    // if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     QMessageBox::warning(this, "Error", "Could not open " + path);
    //     return;
    // }

    std::string stdpath = path.toStdString();
    DataTable table(stdpath);
}

