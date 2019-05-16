#include "fortuneserver.h"
#include <stdlib.h>

FortuneServer::FortuneServer(QObject *parent, int max) : QTcpServer(parent)
{
    if(max < 1) max=1;
    qDebug() << "Max threads:" << max;
    pool = new QThreadPool(this);
    pool->setMaxThreadCount(max);
    setMaxPendingConnections(max);
}

void FortuneServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "incomingConnection()";

    Worker *task = new Worker();
    task->setAutoDelete(true);
    task->socketDescriptor = socketDescriptor;
    if(!pool->tryStart(task)){
        qDebug() << "Unable to start the worker";
        QTcpSocket s;
        s.setSocketDescriptor(socketDescriptor);
        s.write("BUSY\r\n");
        s.flush();
        s.close();
    }
    else qDebug() << "pool started";

}
