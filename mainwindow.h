#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class Worker :  public QObject
{
    Q_OBJECT

public:
    Worker();
    ~Worker();

public slots:
    void startTimer();
    void stopTimer();
    void onTimeout();

signals:
    void updateTime(QString time);

private:
    QTimer* timer;
    int elapsed;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_button_clicked();
    void updateUI(QString time);

signals:
    void startWorker();
    void stopWorker();

private:
    Ui::MainWindow *ui;
    QThread* thread;
    Worker* worker;
};

#endif // MAINWINDOW_H
