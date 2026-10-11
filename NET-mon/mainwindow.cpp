#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QRandomGenerator>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    series = new QLineSeries();

    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Пакетов в секунду");
    chart->legend()->hide();
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, 60);
    chart->axes(Qt::Vertical).first()->setRange(0, 100);

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    auto *layout = new QVBoxLayout(ui->chartWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);

    // фейковые данные раз в секунду, пока нет реального перехвата
    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        series->append(tick++, QRandomGenerator::global()->bounded(100));
        if (tick > 60)
            chart->axes(Qt::Horizontal).first()->setRange(tick - 60, tick);
    });
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}