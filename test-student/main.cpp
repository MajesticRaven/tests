#include "mainmenu.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss(":/images/styles.qss");
    qss.open(QFile::ReadOnly);

    QString buf = qss.readAll();
    a.setStyleSheet(buf);
    a.topLevelWindows();

    MainMenu w;

    w.setWindowFlag(Qt::WindowStaysOnTopHint);
    w.showFullScreen();

    return a.exec();
}
