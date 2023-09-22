#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void wheelEvent(QWheelEvent* event) override;

private slots:
    void do_scrollBarChange(int value);

    void on_actOpenDB_triggered();

    void on_actAddData_triggered();

private:
    void    selectData();       //查询数据

private:
    QSqlDatabase  DB;   //数据库
    QSqlQueryModel  *qryModel;  //数据模型
    int mTotalRows = 0;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
