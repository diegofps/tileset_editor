#include "appstyles.h"
#include <QFile>

QString & AppStyles::get(QString key)
{
    if (_cachedStylesheets.contains(key))
        return _cachedStylesheets[key];

    QFile file(QString(":/style/") + key + ".qss");
    file.open(QFile::ReadOnly);

    if (!file.isReadable())
    {
        qWarning("Could not read style file %s", qUtf8Printable(key));
        _cachedStylesheets[key] = "";
    }
    else
    {
        QString stylesheet = QLatin1String(file.readAll());
        _cachedStylesheets[key] = stylesheet;
    }

    return _cachedStylesheets[key];
}
