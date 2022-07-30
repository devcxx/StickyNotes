#include "mainwindow.h"
#include "singleinstance.h"
#include <QApplication>
#include <QTranslator>
#include <QVersionNumber>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    // Prevent many instances of the app to be launched
    QString name = "com.xingyao.stickynotes";
    SingleInstance instance;
    if (instance.hasPrevious(name)) {
        return EXIT_SUCCESS;
    }

    instance.listen(name);

    QTranslator translator1;
    if (translator1.load(":/translations/qt_zh_CN")) {
        QCoreApplication::installTranslator(&translator1);
    }

    QTranslator translator2;
    if (translator2.load(":/translations/zh_CN")) {
        QCoreApplication::installTranslator(&translator2);
    }

    QCoreApplication::setApplicationName(QCoreApplication::translate("main", "StickyNotes"));

    QVersionNumber appVersion(1, 0, 0);
    QCoreApplication::setApplicationVersion(appVersion.toString());

    MainWindow mw;
    mw.show();

    // Bring the Notes window to the front
    QObject::connect(&instance, &SingleInstance::newInstance, [&]() {
        (&mw)->show();
    });

    return a.exec();
}
