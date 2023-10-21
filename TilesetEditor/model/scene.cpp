#include "scene.h"
#include "errors.h"

Scene::Scene() : id(0)
{

}

Scene::Scene(QJsonObject & data)
{
    // ID

    if (!data.contains("ID"))
        throw ContextError("Cluster is missing attribute 'ID'");

    if (!data["ID"].isDouble())
        throw ContextError("Cluster has the wrong value type associated to 'ID'");

    id = data["ID"].toInt();

    // Name

    if (!data.contains("Name"))
        throw ContextError("Cluster is missing attribute 'Name'");

    if (!data["Name"].isString())
        throw ContextError("Cluster has the wrong value associated to 'Name'");

    name = data["Name"].toString();

}

QJsonObject Scene::exportAsJson()
{
    QJsonObject jTile;
    jTile["ID"] = id;
    jTile["Name"] = name;

    return jTile;
}
