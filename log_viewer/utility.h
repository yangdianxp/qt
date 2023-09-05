#ifndef UTILITY_H
#define UTILITY_H
#include <QDebug>
#include <QThread>
#include <QCoreApplication>

#define MYLOG qDebug() << "[" << __FILE__ << ":" << __LINE__ << ":" << __func__ \
                        << ":" << QCoreApplication::applicationPid()            \
                        << ":" << QThread::currentThreadId() << "]"

#endif // UTILITY_H
