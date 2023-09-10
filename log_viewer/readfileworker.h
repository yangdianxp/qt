#ifndef READFILEWORKER_H
#define READFILEWORKER_H
#include <QObject>
#include <QFile>

class MainWindow;

class ReadFileWorker : public QObject
{
    Q_OBJECT
public:
    ReadFileWorker(MainWindow* mainWindow);

signals:
    void ReadLine(QString content);
    void ClearPlainTextEditFile();
    void SetScrollBarFileValue(int value);
public slots:
    void OpenFile(QString fileName);
    void ReadFilePos(int lineNo);
private:
    QString mFileName;
    std::shared_ptr<QFile> mFile;
    MainWindow* mMainWindow;
};

#endif // READFILEWORKER_H
