#include "readfileworker.h"
#include "utility.h"

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <string>

void ReadFileWorker::Scan(QString fileName)
{
    QList<qint64> indexs;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    for (int i = 0; i < 5; ++i)
    {
        indexs.append(file.pos());
        file.readLine();
    }

    mFileName = fileName;
    mFile = std::make_shared<QFile>(mFileName);
    if (!mFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    qint64 endPos = mFile->size();

    emit ClearPlainTextEditFile();

    QByteArray data;
    int lineNo = 1;
    bool alreadyUpdate = false;
    mLineNoPos[lineNo++] = 0;
    QByteArray showData;
    while (!mFile->atEnd()) {
        qint64 pos = mFile->pos();
        data = mFile->read(16777216);  // 读取大块数据
        while (true) {
            int data_pos = data.indexOf('\n');
            if (data_pos == -1) break;
            pos = pos + data_pos + 1;
            mLineNoPos[lineNo++] = pos;
            QByteArray lineData = data.left(data_pos + 1);
            showData.append(lineData);
            if (lineNo > 300)
            {
                emit ReadLine(showData);
                emit SetScrollBarFileValue(0);
                alreadyUpdate = true;
            }
            data.remove(0, data_pos + 1);
        }
        MYLOG << "scan lineNo:" << lineNo;
        emit ChangeScrollBarFileMaximum(lineNo);
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


//    std::string data(16777216, '\0');
//    int lineNo = 1;
//    bool alreadyUpdate = false;
//    mLineNoPos[lineNo++] = 0;
//    while (!mFile->atEnd()) {
//        qint64 pos = mFile->pos();
//        qint64 len = mFile->read(data.data(), 16777216);
//        if (len < 16777216)
//        {
//            data.resize(len);
//        }
//        std::size_t data_pos = data.find("\n");
//        while (data_pos != std::string::npos) {
//            mLineNoPos[lineNo++] = pos + data_pos + 1;
//            if (lineNo > 300 && !alreadyUpdate)
//            {
//                emit ReadLine(QString::fromStdString(data.substr(0, data_pos + 1)));
//                emit SetScrollBarFileValue(0);
//                alreadyUpdate = true;
//            }
//            data_pos = data.find("\n", data_pos + 1);
//        }
//        MYLOG << "scan lineNo:" << lineNo;
//        emit ChangeScrollBarFileMaximum(lineNo);
//        int progress = ((double)pos / (double)endPos) * 1000.0;
//        MYLOG << "scan lineNo:" << lineNo << " progress:" << progress;
//        emit SetReadFileProgress(progress);
//    }
//    emit ChangeScrollBarFileMaximum(lineNo-1);
//    emit SetReadFileProgress(1000);
//    if (!alreadyUpdate)
//    {
//        emit ReadLine(QString::fromStdString(data));
//        emit SetScrollBarFileValue(0);
//        alreadyUpdate = true;
//    }
//    MYLOG << "scan end.";
}

void ReadFileWorker::ReadFilePos(int lineNo)
{
    if (mFile)
    {
        std::string data(4096, '\0');
        emit ClearPlainTextEditFile();
        mFile->seek(mLineNoPos[lineNo]);
        // mFile->seek(24);
        QString readData;
        int lineNo = 1;
        while (!mFile->atEnd())
        {
            QString line_data = mFile->readLine();
            readData.append(line_data);
            if (++lineNo > 300)
            {
                break;
            }
        }
        // mFile->read(data.data(), 4096);
        emit ReadLine(readData);
        emit SetScrollBarFileValue(0);
    }
}
