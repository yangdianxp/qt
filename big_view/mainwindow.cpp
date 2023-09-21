#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include    <QFileDialog>
#include    <QMessageBox>
#include    <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_scrollBarChange(int value)
{
    QSqlQuery sqlQuery;
    sqlQuery.prepare("SELECT id, name, content FROM person WHERE id >= :id limit 10");
    sqlQuery.bindValue(":id", value);
    sqlQuery.exec();
    qryModel->setQuery(std::move(sqlQuery));
    if (qryModel->lastError().isValid())
    {
        QMessageBox::critical(this, "错误", "数据表查询错误,错误信息\n"
                              +qryModel->lastError().text());
        return;
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
    sqlQuery.prepare("SELECT id, name, content FROM person WHERE id >= :id limit 10");
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
    int totalRows = sqlQuery1.value(0).toInt();
    qDebug() << totalRows;
    ui->verticalScrollBar->setMaximum(totalRows);

    connect(ui->verticalScrollBar, &QScrollBar::valueChanged, this, &MainWindow::do_scrollBarChange);
}

