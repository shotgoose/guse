#include "mainwindow.h"
#include "constants.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->setMovable(true);

    setWindowTitle(QString("GUSE v%1").arg(App::version));

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, [this](int i) {
        QWidget *page = ui->tabWidget->widget(i);
        ui->tabWidget->removeTab(i);
        page->deleteLater();
    });

    QVector<double> x, y;
    for (int i = 0; i <= 50; ++i) {
        x.append(i / 5.0);
        y.append(cos(x.last()));
    }

    // ui->plot->addGraph();
    // ui->plot->graph(0)->setData(x, y);
    // ui->plot->xAxis->setLabel("x");
    // ui->plot->yAxis->setLabel("y");
    // ui->plot->rescaleAxes();
    // ui->plot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

