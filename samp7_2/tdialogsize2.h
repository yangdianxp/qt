#ifndef TDIALOGSIZE2_H
#define TDIALOGSIZE2_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {class TDialogSize2;}
QT_END_NAMESPACE

class TDialogSize2 : public QDialog
{
    Q_OBJECT

public:
    TDialogSize2(QWidget *parent = 0);
    ~TDialogSize2();

private slots:

private:
    Ui::TDialogSize2 *ui;
};

#endif // TDIALOGSIZE2_H
