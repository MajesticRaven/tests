#include "mainmenu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/res/style.qss");
    qss.open(QFile::ReadOnly);

    a.setStyleSheet(qss.readAll());
    MainMenu w;
    w.show();

    return a.exec();
}
