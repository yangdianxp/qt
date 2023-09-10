#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utility.h"

#include <QFileDialog>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->centerSplitter);
    mReadFileProgress = new QProgressBar(this);
    mReadFileProgress->setMinimum(0);
    mReadFileProgress->setMaximum(1000);
    ui->statusbar->addWidget(mReadFileProgress);

    ui->verticalScrollBarFile->setMinimum(1);

    mScanFileWorker = std::make_shared<ScanFileWorker>();
    mScanFileWorker->moveToThread(&mScanFileThread);
    connect(this, &MainWindow::fileNameObtained, mScanFileWorker.get(), &ScanFileWorker::Scan);
    mScanFileThread.start();

    mReadFileWorker = std::make_shared<ReadFileWorker>(this);
    mReadFileWorker->moveToThread(&mReadFileThread);
    connect(this, &MainWindow::fileNameObtained, mReadFileWorker.get(), &ReadFileWorker::OpenFile);
    mReadFileThread.start();

    connect(mScanFileWorker.get(), &ScanFileWorker::ReadLine, ui->plainTextEditFile, &QPlainTextEdit::appendPlainText);
    connect(mScanFileWorker.get(), &ScanFileWorker::ClearPlainTextEditFile, ui->plainTextEditFile, &QPlainTextEdit::clear);
    connect(mScanFileWorker.get(), &ScanFileWorker::ChangeScrollBarFileMaximum, this, &MainWindow::do_changeScrollBarFileMaximum);
    connect(mScanFileWorker.get(), &ScanFileWorker::SetScrollBarFileValue, ui->plainTextEditFile->verticalScrollBar(), &QScrollBar::setValue);
    connect(mScanFileWorker.get(), &ScanFileWorker::SetReadFileProgress, mReadFileProgress, &QProgressBar::setValue);

    connect(mReadFileWorker.get(), &ReadFileWorker::ReadLine, ui->plainTextEditFile, &QPlainTextEdit::appendPlainText);
    connect(mReadFileWorker.get(), &ReadFileWorker::ClearPlainTextEditFile, ui->plainTextEditFile, &QPlainTextEdit::clear);
    connect(mReadFileWorker.get(), &ReadFileWorker::SetScrollBarFileValue, ui->plainTextEditFile->verticalScrollBar(), &QScrollBar::setValue);
    connect(ui->verticalScrollBarFile, &QScrollBar::valueChanged, mReadFileWorker.get(), &ReadFileWorker::ReadFilePos);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ClearLineNoPos()
{
    QMutexLocker locker(&mLineNoPosMutex);
    mLineNoPos.clear();
}

void MainWindow::AddLineNoPos(int lineNo, qint64 pos)
{
    QMutexLocker locker(&mLineNoPosMutex);
    mLineNoPos[lineNo] = pos;
}

qint64 MainWindow::GetLineNoPos(int lineNo)
{
    QMutexLocker locker(&mLineNoPosMutex);
    return mLineNoPos[lineNo];
}

void MainWindow::do_changeScrollBarFileMaximum(int value)
{
    ui->verticalScrollBarFile->setMaximum(value);
}

void MainWindow::on_actOpen_triggered()
{
    MYLOG << "on_actOpen_triggered";

    QScrollBar *vScrollBar = ui->plainTextEditFile->verticalScrollBar();
    vScrollBar->setValue(500);

    QString file= QFileDialog::getOpenFileName(this, "选择文件", "", "(*)");
    if (file.isEmpty())
        return;

    emit fileNameObtained(file);
}

