#ifndef APPSTYLES_H
#define APPSTYLES_H

#include <QString>
#include <QMap>

class AppStyles
{
private:

    QMap<QString, QString> cachedStylesheets;

public:

    QString get(QString key);

};

#endif // APPSTYLES_H
