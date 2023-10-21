#include "cluster.h"
#include "errors.h"

Cluster::Cluster() : id(0)
{

}

Cluster::Cluster(QJsonObject & data)
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

QJsonObject Cluster::exportAsJson()
{
    QJsonObject jTile;
    jTile["ID"] = id;
    jTile["Name"] = name;

    return jTile;
}
