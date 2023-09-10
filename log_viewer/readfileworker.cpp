#include "readfileworker.h"
#include "mainwindow.h"

#include <QScrollBar>

ReadFileWorker::ReadFileWorker(MainWindow* mainWindow) : mMainWindow(mainWindow)
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
        std::string data(4096, '\0');
        qint64 pos = mMainWindow->GetLineNoPos(lineNo);
        mFile->seek(pos);
        mFile->read(data.data(), 4096);
        emit ClearPlainTextEditFile();
        emit ReadLine(QString::fromStdString(data));
        emit SetScrollBarFileValue(0);
    }
}
