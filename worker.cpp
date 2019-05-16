#include "worker.h"
#include <QDebug>
#include <QImage>
#include <QPen>
#include <QFont>
#include <QPainter>
#include <QBuffer>

Worker::Worker()
{

}

void Worker::run()
{
    char buf[1024];
    if(!socketDescriptor) return;

    QTcpSocket socket;
    socket.setSocketDescriptor(socketDescriptor);
    qDebug() << "incoming socket...";
    socket.write("READY\r\n");
    socket.waitForBytesWritten();

    socket.waitForReadyRead();
    socket.readLine(buf, 1024);

    QString request = QString::fromLocal8Bit(buf);
    QStringList parts = request.split(':');
    qDebug() << "Request:" << request;

    if(parts[0] == "POST"){
        qDebug() << "post cmd";
        int sz = parts[3].toInt();
        qDebug() << sz << "bytes";
        QString text = parts[1];
        qDebug() << "Text to write:" << text;

        uchar *buf2;
        if(getFile(socket, sz, &buf2)){
            qDebug() << "image received!";

            QImage jpegImage;
            bool jpgLoad = jpegImage.loadFromData(buf2, sz);
            if(!jpgLoad) qDebug() << "parse error!";
            else qDebug() << "parsed:" << jpegImage.width() << jpegImage.height();

            // осталось вставить текст и послать обратно
            QPainter p(&jpegImage);

            p.setPen(QPen(Qt::black));
            QFont ff = QFont("Arial", 100);
            if(jpegImage.width() < 2000) p.setFont(QFont("Arial", 24)); // размер фонта должен зависеть от длины строки и ширины картинки
            else p.setFont(ff);

            p.drawText(jpegImage.rect(), Qt::AlignBottom | Qt::AlignHCenter, text); // теневой текст
            QRect shadow = jpegImage.rect();
            shadow.setBottom(shadow.bottom()-2);    // сдвигаем на пару пикселов вверх и влево
            shadow.setLeft(shadow.left() - 2);

            p.setPen(QPen(Qt::yellow));
            p.drawText(shadow, Qt::AlignBottom | Qt::AlignHCenter, text);           // лицевой текст

            jpegImage.save("tim.jpg", "jpg");   // вместо этого нужно засылать картинку
            QByteArray arr;
            QBuffer buffer(&arr);
            buffer.open(QIODevice::WriteOnly);
            jpegImage.save(&buffer, "JPG");
            qDebug() << "arr size:" << arr.size();  // картинка может стать меньше исходной)

            free(buf2); // освободим память
            QString response = "POST:";
            response += parts[2] + ":" + QString::number(arr.size());
            socket.write(response.toLocal8Bit());
            socket.flush();
            socket.write(arr);
            socket.waitForBytesWritten();
            socket.close();
        } else {
            qDebug() << "image download error";
        }
    }   // end if POST
}

bool Worker::getFile(QTcpSocket &s, qint32 len, uchar **bb){
    uchar *buf = reinterpret_cast<uchar *>(malloc(static_cast<size_t>(len)));
    qint64 rcvd = 0;
    qint64 por = 0;
    while(rcvd < len){
        s.waitForReadyRead();
        por = s.read(reinterpret_cast<char *>(buf+rcvd), len-rcvd);
        rcvd += por;
        qDebug() << "read" << por << "bytes";
        if(!s.isReadable()) break;
    }
    if(rcvd == len){
        *bb = buf;
        return true;
    }
    free(buf);
    return false;
}
