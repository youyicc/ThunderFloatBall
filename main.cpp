#include "ThunderFloatBall.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ThunderFloatBall mainWidget;
    mainWidget.show();
    return a.exec();
}
