#ifndef JSONHELPERS_H
#define JSONHELPERS_H

#include "errors.h"

#include <QColor>
#include <QJsonArray>
#include <QJsonObject>

inline void getBoolOrFail(bool & out, QJsonObject & data, char const * objName, char const * key)
{
    if (!data.contains(key))
        throw ContextError(QString("%1 is missing attribute '%2'.").arg(objName, key));

    if (!data[key].isBool())
        throw ContextError(QString("%1 has the wrong value type associated to '%1'.").arg(objName, key));

    out = data[key].toBool();
}

inline void getIntOrFail(int & out, QJsonObject & data, char const * objName, char const * key)
{
    if (!data.contains(key))
        throw ContextError(QString("%1 is missing attribute '%2'.").arg(objName, key));

    if (!data[key].isDouble())
        throw ContextError(QString("%1 has the wrong value type associated to '%1'.").arg(objName, key));

    out = data[key].toInt();
}

template <typename OUT>
inline void getIntegerOrFail(OUT & out, QJsonObject & data, char const * objName, char const * key)
{
    if (!data.contains(key))
        throw ContextError(QString("%1 is missing attribute '%2'.").arg(objName, key));

    if (!data[key].isDouble())
        throw ContextError(QString("%1 has the wrong value type associated to '%1'.").arg(objName, key));

    out = data[key].toInteger();
}

inline void getQStringOrFail(QString & out, QJsonObject & data, char const * objName, char const * key)
{
    if (!data.contains(key))
        throw ContextError(QString("%1 is missing attribute '%2'.").arg(objName, key));

    if (!data[key].isString())
        throw ContextError(QString("%1 has the wrong value type associated to '%1'.").arg(objName, key));

    out = data[key].toString();
}

inline void getQColorOrFail(QColor & out, QJsonObject & data, char const * objName, char const * key)
{
    if (!data.contains(key))
        throw ContextError(QString("%1 is missing attribute '%2'").arg(objName, key));

    if (!data[key].isArray())
        throw ContextError(QString("%1 has the wrong value type associated to '%2'").arg(objName, key));

    QJsonArray jColor = data[key].toArray();

    if (jColor.size() != 3)
        throw ContextError(QString("%1 has the wrong size for array associated to '%2'").arg(objName, key));

    if (!jColor[0].isDouble() || !jColor[1].isDouble() || !jColor[2].isDouble())
        throw ContextError(QString("%1 has the wrong value type inside '%2'").arg(objName, key));

    int const r = jColor[0].toInt();
    int const g = jColor[1].toInt();
    int const b = jColor[2].toInt();

    out = QColor::fromRgb(r,g,b);
}

inline void getIntOrDefault(int & out, QJsonObject & data, char const * objName, char const * key, int const defaultValue)
{
    (void) objName;
    out = data.contains(key) ? data[key].toInt(defaultValue) : defaultValue;
}

inline void getIntArrayOrFail(int * const out, QJsonObject & data, char const * objName, char const * key, qsizetype const size)
{
    if (!data.contains(key))
        throw ContextError(QString("%1 is missing attribute '%2'").arg(objName, key));

    auto jArray = data[key].toArray();
    if (jArray.size() != size)
        throw ContextError(QString("%1 has the wrong number of values in attribute %2. Expected %3, got %4.")
                           .arg(objName, key).arg(size, jArray.size()));

    for (int i=0;i!=size;++i)
    {
        if (jArray[i].isDouble())
            out[i] = jArray[i].toInt();
        else
            throw ContextError(QString("%1 has the wrong value in attribute '%2', position %3")
                               .arg(objName, key).arg(i));
    }
}

inline void getQColorArrayOrFail(QColor * const out, QJsonObject & data, char const * objName, char const * key, qsizetype const size)
{
    if (!data.contains(key))
        throw ContextError(QString("%1 is missing the attribute %2").arg(objName, key));

    if (!data[key].isArray())
        throw ContextError(QString("%1 has the wrong value type for %2").arg(objName, key));

    QJsonArray jColors = data[key].toArray();

    if (jColors.size() != size)
        throw ContextError(QString("%1 has an invalid size for attribute %2").arg(objName, key));

    for (qsizetype i=0;i!=size;++i)
    {
        QJsonValue jValue = jColors.at(i);

        if (!jValue.isArray())
            throw ContextError(QString("%1 expects an array of arrays in attribute %2").arg(objName, key));

        QJsonArray jColor = jValue.toArray();

        if (jColor.size() != 3)
            throw ContextError(QString("%1 expects inner arrays with size 3 in attribute %2, position %3")
                               .arg(objName, key).arg(i));

        if (!jColor[0].isDouble() || !jColor[1].isDouble() || !jColor[2].isDouble())
            throw ContextError(QString("%1 has the wrong value type inside '%2', position %3")
                               .arg(objName, key).arg(i));

        out[i] = QColor(
                    jColor.at(0).toInt(),
                    jColor.at(1).toInt(),
                    jColor.at(2).toInt());
    }
}

inline void getIntIntQHashOrFail(QHash<int,int> & out, QJsonObject & data, char const * objName, char const * keyName)
{
    if (!data.contains(keyName))
        throw ContextError(QString("%1 is missing the attribute '%2'").arg(objName, keyName));

    if (!data[keyName].isObject())
        throw ContextError(QString("%1 has an invalid value for '%2'").arg(objName, keyName));

    auto jPalettes = data[keyName].toObject();
    out.clear();
    bool ok;

    for (QString const & key : jPalettes.keys())
    {
        int iKey = key.toInt(&ok);
        auto jValue = jPalettes[key];

        if (!ok)
            throw ContextError(QString("%1 has an invalid key %2 in attribute '%3'.").arg(objName, key, keyName));

        if (!jValue.isDouble())
            throw ContextError(QString("%1 has an invalid value %2 in attribute '%3', key %4.")
                               .arg(objName).arg(jValue.toString()).arg(keyName).arg(key));

        out[iKey] = jValue.toInt();
    }
}

#endif // JSONHELPERS_H
