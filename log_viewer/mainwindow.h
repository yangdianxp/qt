#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "readfileworker.h"

#include <QMainWindow>
#include <QThread>
#include <QProgressBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void fileNameObtained(QString fileName);

public slots:
    void do_changeScrollBarFileMaximum(int value);

private slots:
    void on_actOpen_triggered();

private:
    Ui::MainWindow *ui;
    QProgressBar *mReadFileProgress;
    QThread mReadFileThread;
    std::shared_ptr<ReadFileWorker> mReadFileWorker = nullptr;
};
#endif // MAINWINDOW_H
