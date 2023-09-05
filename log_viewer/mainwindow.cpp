#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utility.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->centerSplitter);

    mReadFileWorker = std::make_shared<ReadFileWorker>();
    mReadFileWorker->moveToThread(&mReadFileThread);
    connect(this, &MainWindow::fileNameObtained, mReadFileWorker.get(), &ReadFileWorker::Scan);
    mReadFileThread.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actOpen_triggered()
{
    MYLOG << "on_actOpen_triggered";

    QString file= QFileDialog::getOpenFileName(this, "选择文件", "", "(*)");
    if (file.isEmpty())
        return;

    emit fileNameObtained(file);
}

