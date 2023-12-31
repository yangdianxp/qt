#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include    <QFileDialog>
#include    <QMessageBox>
#include    <QScrollBar>
#include    <QString>
#include    <QRandomGenerator>
#include <QTimer>
#include <QDebug>
#include <QWheelEvent>

QString generateRandomString(int length) {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;
    for(int i=0; i<length; ++i)
    {
        int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->verticalScrollBar->setHidden(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_scrollBarChange(int value)
{
    QSqlQuery sqlQuery;
    sqlQuery.prepare("SELECT id, name, content FROM person WHERE id >= :id limit 100");
    sqlQuery.bindValue(":id", value);
    sqlQuery.exec();
    qryModel->setQuery(std::move(sqlQuery));
    if (qryModel->lastError().isValid())
    {
        QMessageBox::critical(this, "错误", "数据表查询错误,错误信息\n"
                              +qryModel->lastError().text());
        return;
    }

    int pageRows = ui->tableView->verticalScrollBar()->pageStep();
    ui->verticalScrollBar->setMaximum(this->mTotalRows - pageRows + 1);

    if (ui->verticalScrollBar->isHidden())
    {
        int totalHeight = 0;
        for(int i = 0; i < ui->tableView->model()->rowCount(); i++){
            totalHeight += ui->tableView->rowHeight(i);
            if (totalHeight > ui->tableView->height())
            {
                ui->verticalScrollBar->setHidden(false);
                break;
            }
        }
    }
}

void MainWindow::on_actOpenDB_triggered()
{
    //打开数据库
    QString aFile=QFileDialog::getOpenFileName(this,"选择文件","","SQLite数据库(*.db3)");
    if (aFile.isEmpty())
        return;

    DB=QSqlDatabase::addDatabase("QSQLITE");  //添加 SQLITE数据库驱动
    DB.setDatabaseName(aFile);   //设置数据库文件
    if (DB.open())       //成功打开数据库
        selectData();
    else
        QMessageBox::warning(this, "错误", "打开数据库失败");
}

void MainWindow::selectData()
{
    QSqlQuery sqlQuery;
    sqlQuery.prepare("SELECT id, name, content FROM person WHERE id >= :id limit 100");
    sqlQuery.bindValue(":id", 1);
    sqlQuery.exec();

    //1. 创建数据模型，查询数据
    qryModel=new QSqlQueryModel(this);
    qryModel->setQuery(std::move(sqlQuery));
    if (qryModel->lastError().isValid())
    {
        QMessageBox::critical(this, "错误", "数据表查询错误,错误信息\n"
                              +qryModel->lastError().text());
        return;
    }

    QSqlRecord rec=qryModel->record();  //获取一个空记录，为了获取字段序号
    qryModel->setHeaderData(rec.indexOf("id"),  Qt::Horizontal, "行号");
    qryModel->setHeaderData(rec.indexOf("name"),   Qt::Horizontal, "姓名");
    qryModel->setHeaderData(rec.indexOf("content"), Qt::Horizontal, "内容");

    ui->tableView->setModel(qryModel);

    ui->verticalScrollBar->setMinimum(1);
    QSqlQuery sqlQuery1;
    sqlQuery1.prepare("SELECT count(id) FROM person");
    sqlQuery1.exec();
    sqlQuery1.next();
    mTotalRows = sqlQuery1.value(0).toInt();
    qDebug() << mTotalRows;

    int totalHeight = 0;
    for(int i = 0; i < ui->tableView->model()->rowCount(); i++){
        totalHeight += ui->tableView->rowHeight(i);
        if (totalHeight > ui->tableView->height())
        {
            ui->verticalScrollBar->setHidden(false);
            break;
        }
    }

    int pageRows = ui->tableView->verticalScrollBar()->pageStep();
    ui->verticalScrollBar->setMaximum(this->mTotalRows - pageRows + 1);
    connect(ui->verticalScrollBar, &QScrollBar::valueChanged, this, &MainWindow::do_scrollBarChange);

    // 创建一个QTimer对象
    QTimer *timer = new QTimer(this);

    // 连接QTimer的timeout()信号到你想要执行的槽函数
    connect(timer, &QTimer::timeout, this, [this](){
        QSqlQuery query;
        query.prepare("INSERT INTO person (name, content) VALUES (:name, :content)");
        query.bindValue(":name", generateRandomString(5));
        query.bindValue(":content", generateRandomString(10));
        query.exec();
        this->mTotalRows++;
        emit this->ui->verticalScrollBar->valueChanged(this->ui->verticalScrollBar->value());
    });

    timer->start(1000);
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
    if (ui->verticalScrollBar->maximum() >= 2)
    {
        QPoint point = event->angleDelta();
        if (point.y() > 0)
        {
            int setValue = ui->verticalScrollBar->value() - 1;
            if (setValue < 1)
            {
                setValue = 1;
            }
            ui->verticalScrollBar->setValue(setValue);
        }
        else
        {
            int setValue = ui->verticalScrollBar->value() + 1;
            if (setValue > ui->verticalScrollBar->maximum())
            {
                setValue = ui->verticalScrollBar->maximum();
            }
            ui->verticalScrollBar->setValue(setValue);
        }
    }
    QWidget::wheelEvent(event);
}



void MainWindow::on_actAddData_triggered()
{
//    QSqlQuery query;
//    query.prepare("INSERT INTO person (name, content) VALUES (:name, :content)");
//    query.bindValue(":name", generateRandomString(5));
//    query.bindValue(":content", generateRandomString(10));
//    query.exec();

//    QSqlQuery sqlQuery1;
//    sqlQuery1.prepare("SELECT count(id) FROM person");
//    sqlQuery1.exec();
//    sqlQuery1.next();
//    int totalRows = sqlQuery1.value(0).toInt();
//    qDebug() << totalRows;
//    ui->verticalScrollBar->setMaximum(totalRows);

//    emit ui->verticalScrollBar->valueChanged(ui->verticalScrollBar->value());
}

