#ifndef READFILEWORKER_H
#define READFILEWORKER_H
#include <QObject>
#include <QFile>
#include <QHash>

class ReadFileWorker : public QObject
{
    Q_OBJECT
signals:
    void ReadLine(QString content);
    void ChangeScrollBarFileMaximum(int value);
    void ClearPlainTextEditFile();
    void SetScrollBarFileValue(int value);
    void SetReadFileProgress(int value);

public slots:
    void Scan(QString fileName);
    void ReadFilePos(int lineNo);

private:
    // void ReadContents();

private:
    QString mFileName;
    QHash<int, qint64> mLineNoPos;
    std::shared_ptr<QFile> mFile;
};

#endif // READFILEWORKER_H
