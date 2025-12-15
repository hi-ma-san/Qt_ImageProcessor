#include "ip.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IP w;
    w.show();
    return a.exec();
}
