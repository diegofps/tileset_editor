#include "palette.h"
#include "errors.h"

#include <QJsonArray>

Palette::Palette(QJsonObject & data)
{
    id = data["ID"].toInt();
    size = data["Size"].toInt();
    frequency = data["Frequency"].toInt();

    if (!data.contains("Colors"))
        throw ContextError("Palette is missing the attribute Colors");

    if (!data["Colors"].isArray())
        throw ContextError("Palette has the wrong value type for Colors");

    QJsonArray jColors = data["Colors"].toArray();

    if (jColors.size() != size)
        throw ContextError("Invalid size of Palette attribute Colors");

    for (int i=0;i!=size;++i)
    {
        QJsonValue jValue = jColors.at(i);

        if (!jValue.isArray())
            throw ContextError("A Palette color must be an array with 3 values");

        QJsonArray jColor = jValue.toArray();

        if (jColor.size() != size)
            throw ContextError("A Palette color must be an array with 3 values");

        colors[i] = QColor(
                    jColor.at(0).toInt(),
                    jColor.at(1).toInt(),
                    jColor.at(2).toInt());
    }
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

//void Palette::import(Palette *other)
//{
//    frequency += other->frequency;
//}
