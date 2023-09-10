#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scanfileworker.h"
#include "readfileworker.h"

#include <QMainWindow>
#include <QThread>
#include <QProgressBar>
#include <QMutex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void ClearLineNoPos();
    void AddLineNoPos(int lineNo, qint64 pos);
    qint64 GetLineNoPos(int lineNo);

signals:
    void fileNameObtained(QString fileName);

public slots:
    void do_changeScrollBarFileMaximum(int value);

private slots:
    void on_actOpen_triggered();

private:
    Ui::MainWindow *ui;
    QProgressBar *mReadFileProgress;
    QThread mScanFileThread;
    std::shared_ptr<ScanFileWorker> mScanFileWorker = nullptr;
    QThread mReadFileThread;
    std::shared_ptr<ReadFileWorker> mReadFileWorker = nullptr;
    QMutex mLineNoPosMutex;
    QHash<int, qint64> mLineNoPos;
};
#endif // MAINWINDOW_H
