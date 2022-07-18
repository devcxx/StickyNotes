#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QWidget>
#include <QXmlStreamReader>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainWindow mw;
    mw.show();

    return a.exec();
}
