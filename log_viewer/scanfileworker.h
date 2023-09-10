#ifndef SCANFILEWORKER_H
#define SCANFILEWORKER_H
#include <QObject>

class ScanFileWorker : public QObject
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

private:
};

#endif // SCANFILEWORKER_H
