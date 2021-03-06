#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include "fortuneserver.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class Dialog : public QWidget
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr, int max = 3);

private:
    QLabel *statusLabel;
    QPushButton *quitButton;
    FortuneServer *server;
    int _t; // число потоков
};

#endif
