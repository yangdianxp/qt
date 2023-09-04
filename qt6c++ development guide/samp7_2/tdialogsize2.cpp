#include "tdialogsize2.h"
#include "ui_tdialogsize2.h"

#include    <QMessageBox>

TDialogSize2::TDialogSize2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDialogSize2)
{
    ui->setupUi(this);
}

TDialogSize2::~TDialogSize2()
{
    QMessageBox::information(this,"提示","TDialogSize对话框被删除");
    delete ui;
}


