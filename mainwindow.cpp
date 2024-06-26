#include "mainwindow.h"
#include "ui_mainwindow.h"

Worker::Worker()
    : timer(nullptr), elapsed(0)
{

}

Worker::~Worker()
{
    delete timer;
}

void Worker::startTimer()
{
    if (!timer)
    {
        timer = new QTimer;
        connect(timer, &QTimer::timeout, this, &Worker::onTimeout);
        elapsed = 0;
    }
    else
    {
        elapsed = 0;
        timer->start(1000);
    }
}

void Worker::stopTimer()
{
    if (timer)
    {
        timer->stop();
    }
}

void Worker::onTimeout()
{
    elapsed += 1;
    emit updateTime(QString::number(elapsed));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , thread(new QThread)
    , worker(new Worker)
{
    ui->setupUi(this);
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &Worker::startTimer);
    connect(this, &MainWindow::startWorker, worker, &Worker::startTimer);
    connect(this, &MainWindow::stopWorker, worker, &Worker::stopTimer);
    connect(worker, &Worker::updateTime, this, &MainWindow::updateUI);

    thread->start();
}

MainWindow::~MainWindow()
{
    thread->quit();
    thread->wait();
    delete worker;
    delete thread;
    delete ui;
}

void MainWindow::on_button_clicked()
{
    QString buttonText = ui->button->text();
    if (buttonText == "Start")
    {
        ui->button->setText("Stop");
        emit startWorker();

    }
    else if (buttonText == "Stop")
    {
        ui->button->setText("Start");
        emit stopWorker();
    }
}

void MainWindow::updateUI(QString time)
{
    int numSecs = time.toInt();
    int numHours = numSecs / 3600;
    numSecs -= (numHours * 3600);
    int numMins = numSecs / 60;
    numSecs -= (numMins * 60);
    QString formattedTime = QString("%1").arg(numHours, 2, 10, QChar('0')) + ":"
                            + QString("%1").arg(numMins, 2, 10, QChar('0')) + ":"
                            + QString("%1").arg(numSecs, 2, 10, QChar('0'));

    ui->time->setText("Time: " + formattedTime);
}
