#include "readfileworker.h"
#include "utility.h"

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>

void ReadFileWorker::Scan(QString fileName)
{
    mFileName = fileName;
    mFile = std::make_shared<QFile>(mFileName);
    if (!mFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    emit ClearPlainTextEditFile();
    mTextStream = std::make_shared<QTextStream>(mFile.get());
    int lineNo = 1;
    while (!mTextStream->atEnd()) {
        qint64 pos = mTextStream->pos();
        mLineNoPos[lineNo] = pos;
        if (lineNo < 300)
        {
            QString line = mTextStream->readLine();
            emit ReadLine(line);
            emit SetScrollBarFileValue(1);
        } else
        {
            mTextStream->readLine();
        }
        if (lineNo % 100 == 0)
        {
            MYLOG << "scan lineNo:" << lineNo;
            emit ChangeScrollBarFileMaximum(lineNo);
        }
        if (!mTextStream->atEnd())
        {
            lineNo++;
        }
    }
    emit ChangeScrollBarFileMaximum(lineNo);
    MYLOG << "scan end.";
}

void ReadFileWorker::ReadFilePos(int lineNo)
{
    if (mTextStream)
    {
        emit ClearPlainTextEditFile();
        mTextStream->seek(mLineNoPos[lineNo]);
        int tempLineNo = 1;
        while (!mTextStream->atEnd()) {
            if (tempLineNo < 300)
            {
                QString line = mTextStream->readLine();
                emit ReadLine(line);
                emit SetScrollBarFileValue(1);
            }
        }
    }
}
