#include <QApplication>
#include <QtCore>

#include <stdlib.h>

#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    int _threads = 3;   // если ключ не задан

    if(argc > 2){
        for(int i=1; i<argc; i++){
            if(strcmp(argv[i], "-threads")==0){
                _threads = atoi(argv[i+1]);
                if(_threads < 3) _threads = 3;  // минимальное число потоков
            }
        }
    }
    Dialog dialog(nullptr, _threads);
    dialog.show();
    return app.exec();
}
