#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QByteArray>
#include <QString>

class Screenshot
{
public:

    int id;
    QString filename;
    QByteArray data;

public:

    Screenshot();

};

#endif // SCREENSHOT_H
