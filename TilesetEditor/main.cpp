#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QFile stylesheetFile(":/style/stylesheet.qss");
    stylesheetFile.open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(stylesheetFile.readAll());

    QApplication a(argc, argv);
    a.setStyleSheet(stylesheet);
    MainWindow w;
    w.show();
    return a.exec();
}
