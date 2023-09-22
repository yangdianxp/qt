#include "mytableview.h"
#include <QWheelEvent>

MyTableView::MyTableView(QWidget *parent) : QTableView(parent)
{

}


void MyTableView::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}
