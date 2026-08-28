#include "mainwindow.h"

#include <QApplication>
#include <QtNoidApp/QtNoidApp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // auto test = QtNoid::App::Core::appExeOrAppBundleDirPath();
    // qDebug() << __func__ << test;
    // qDebug() << __func__ << appConfig->fileName();

    MainWindow w;
    w.show();
    return a.exec();
}
