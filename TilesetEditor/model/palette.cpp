#include "palette.h"
#include "jsonhelpers.h"

#include <QJsonArray>

Palette::Palette(QJsonObject & data)
{
    getIntOrFail(id, data, "Palette", "ID");
    getIntOrFail(size, data, "Palette", "Size");
    getIntOrFail(frequency, data, "Palette", "Frequency");
    getQColorArrayOrFail(colors, data, "Palette", "Colors", size);
}

QJsonObject Palette::exportAsJson()
{
    QJsonArray jColors;

    for (int i=0;i!=size;++i)
    {
        auto & color = colors[i];
        QJsonArray jColor;
        jColor.append(color.red());
        jColor.append(color.green());
        jColor.append(color.blue());
        jColors.append(jColor);
    }

    QJsonObject data;
    data["ID"] = id;
    data["Size"] = size;
    data["Frequency"] = frequency;
    data["Colors"] = jColors;

    return data;
}

QByteArray & Palette::uniqueKey()
{
    if (_uniqueKey.isEmpty())
    {
        _uniqueKey.resize(size*3);
        unsigned char * dst = (unsigned char *) _uniqueKey.data();

        for (int i=0;i!=size;++i,dst+=3)
        {
            QColor & color = colors[i];
            dst[0] = color.red();
            dst[1] = color.green();
            dst[2] = color.blue();
        }
    }

    return _uniqueKey;
}
