#ifndef WORKER_H
#define WORKER_H

#include <QRunnable>
#include <QTcpSocket>

class Worker : public QRunnable
{
public:
    Worker();
    qintptr socketDescriptor;
protected:
    void run() override;
private:
    bool getFile(QTcpSocket &s, qint32 len, uchar **bb);

};

#endif // WORKER_H
