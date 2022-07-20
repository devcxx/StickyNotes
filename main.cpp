#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QVersionNumber>

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

    QVersionNumber appVersion(1, 0, 0);
    QCoreApplication::setApplicationVersion(appVersion.toString());

    MainWindow mw;
    mw.show();

    return a.exec();
}
