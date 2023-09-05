#include "readfileworker.h"
#include "utility.h"

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>

void ReadFileWorker::Scan(QString fileName)
{
    mFileName = fileName;
    QFile file(mFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&file); // 使用 QTextStream 读取文本文件
    QList<qint64> positions;
    while (!in.atEnd()) {
        qint64 pos = in.pos();
        positions.append(pos);
        QString line = in.readLine();
        MYLOG << line;
    }
}
