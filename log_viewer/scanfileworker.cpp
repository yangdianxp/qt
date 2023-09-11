#include "scanfileworker.h"
#include "utility.h"
#include "mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <string>

void ScanFileWorker::Scan(QString fileName)
{
    MainWindow* mainWindow = qobject_cast<MainWindow*>(sender());

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    emit ClearPlainTextEditFile();

    qint64 endPos = file.size();
    QByteArray data;
    int lineNo = 1;
    bool alreadyUpdate = false;
    mainWindow->ClearLineNoPos();
    mainWindow->AddLineNoPos(lineNo++, 0);
    QByteArray showData;
    while (!file.atEnd()) {
        qint64 pos = file.pos();
        data = file.read(16777216);  // 读取大块数据
        while (true) {
            int data_pos = data.indexOf('\n');
            if (data_pos == -1) break;
            pos = pos + data_pos + 2;
            mainWindow->AddLineNoPos(lineNo++, pos);
            QByteArray lineData = data.left(data_pos + 1);
            if (!alreadyUpdate)
            {
                showData.append(lineData);
            }
            if (!alreadyUpdate && lineNo > 300)
            {
                emit ReadLine(showData);
                emit SetScrollBarFileValue(0);
                showData.clear();
                alreadyUpdate = true;
            }
            data.remove(0, data_pos + 1);
        }
        MYLOG << "scan lineNo:" << lineNo;
        emit ChangeScrollBarFileMaximum(lineNo - 1);
        int progress = ((double)pos / (double)endPos) * 1000.0;
        MYLOG << "scan lineNo:" << lineNo << " progress:" << progress;
        emit SetReadFileProgress(progress);
    }
    emit ChangeScrollBarFileMaximum(lineNo-1);
    emit SetReadFileProgress(1000);
    if (!alreadyUpdate)
    {
        emit ReadLine(showData);
        emit SetScrollBarFileValue(0);
        alreadyUpdate = true;
    }
    MYLOG << "scan end.";
}
