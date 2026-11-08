#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVector<double> x, y;
    for (int i = 0; i <= 50; ++i) {
        x.append(i / 5.0);
        y.append(cos(x.last()));
    }

    ui->plot->addGraph();
    ui->plot->graph(0)->setData(x, y);
    ui->plot->xAxis->setLabel("x");
    ui->plot->yAxis->setLabel("y");
    ui->plot->rescaleAxes();
    ui->plot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
