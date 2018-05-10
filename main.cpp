#include "mis.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MIS w;
    w.show();

    return a.exec();
}
