#ifndef READFILEWORKER_H
#define READFILEWORKER_H
#include <QObject>

class ReadFileWorker : public QObject
{
    Q_OBJECT
public slots:
    void Scan(QString fileName);

signals:
    // void ReadComplete(QList<QString> contents);

private:
    // void ReadContents();

private:
    QString mFileName;
};

#endif // READFILEWORKER_H
