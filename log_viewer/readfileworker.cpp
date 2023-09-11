#include "readfileworker.h"
#include "mainwindow.h"

#include <QScrollBar>

ReadFileWorker::ReadFileWorker(MainWindow* mainWindow) : mMainWindow(mainWindow), mDataCache(8192, '\0')
{

}

void ReadFileWorker::OpenFile(QString fileName)
{
    mFileName = fileName;
    mFile = std::make_shared<QFile>(mFileName);
    if (!mFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
}

void ReadFileWorker::ReadFilePos(int lineNo)
{
    if (mFile)
    {
        memset(mDataCache.data(), 0, mDataCache.size());
        qint64 pos = mMainWindow->GetLineNoPos(lineNo);
        mFile->seek(pos);
        qint64 readLen = mFile->read(mDataCache.data(), mDataCache.size());
        emit ClearPlainTextEditFile();
        emit ReadLine(QString::fromUtf8(mDataCache.data(), readLen));
        emit SetScrollBarFileValue(0);
    }
}
