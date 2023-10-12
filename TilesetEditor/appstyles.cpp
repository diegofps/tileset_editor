#include "appstyles.h"
#include <QFile>

QString AppStyles::get(QString key)
{
    if (cachedStylesheets.contains(key))
        return cachedStylesheets.value(key);

    QFile file(QString(":/style/") + key + ".qss");
    file.open(QFile::ReadOnly);

    if (!file.isReadable())
    {
        qWarning("Could not read style file %s", qUtf8Printable(key));
        cachedStylesheets[key] = "";
        return "";
    }

    QString stylesheet = QLatin1String(file.readAll());
    cachedStylesheets[key] = stylesheet;
    return stylesheet;
}
