#pragma warning(push, 3)
#include <QApplication>
#pragma warning(pop)
#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller w;
    w.show();
    return a.exec();
}
