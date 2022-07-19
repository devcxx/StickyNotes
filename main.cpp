#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QTranslator translator1;
    if (translator1.load(":/translations/qt_zh_CN")) {
        QCoreApplication::installTranslator(&translator1);
    }

    QTranslator translator2;
    if (translator2.load(":/translations/zh_CN")) {
        QCoreApplication::installTranslator(&translator2);
    }

    MainWindow mw;
    mw.show();

    return a.exec();
}
