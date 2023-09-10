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

    mReadFileWorker = std::make_shared<ReadFileWorker>();
    mReadFileWorker->moveToThread(&mReadFileThread);
    connect(this, &MainWindow::fileNameObtained, mReadFileWorker.get(), &ReadFileWorker::Scan);
    mReadFileThread.start();

    connect(mReadFileWorker.get(), &ReadFileWorker::ReadLine, ui->plainTextEditFile, &QPlainTextEdit::appendPlainText);
    connect(mReadFileWorker.get(), &ReadFileWorker::ClearPlainTextEditFile, ui->plainTextEditFile, &QPlainTextEdit::clear);
    connect(mReadFileWorker.get(), &ReadFileWorker::ChangeScrollBarFileMaximum, this, &MainWindow::do_changeScrollBarFileMaximum);
    connect(mReadFileWorker.get(), &ReadFileWorker::SetScrollBarFileValue, ui->plainTextEditFile->verticalScrollBar(), &QScrollBar::setValue);
    connect(mReadFileWorker.get(), &ReadFileWorker::SetReadFileProgress, mReadFileProgress, &QProgressBar::setValue);
    connect(ui->verticalScrollBarFile, &QScrollBar::valueChanged, mReadFileWorker.get(), &ReadFileWorker::ReadFilePos);
}

MainWindow::~MainWindow()
{
    delete ui;
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

