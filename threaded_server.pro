QT += network widgets

HEADERS       = dialog.h \
                fortuneserver.h \
                worker.h
SOURCES       = dialog.cpp \
                fortuneserver.cpp \
                main.cpp \
                worker.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/threaded_server
INSTALLS += target


