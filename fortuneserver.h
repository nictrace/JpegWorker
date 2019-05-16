#ifndef FORTUNESERVER_H
#define FORTUNESERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QThreadPool>
#include "worker.h"

//! [0]
class FortuneServer : public QTcpServer
{
    Q_OBJECT

public:
    FortuneServer(QObject *parent = nullptr, int max = 3);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QStringList fortunes;
    QThreadPool *pool;
};
//! [0]

#endif
