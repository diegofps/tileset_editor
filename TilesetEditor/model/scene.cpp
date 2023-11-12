#include "scene.h"
#include "helpers/json.h"

Scene::Scene() : id(0)
{

}

Scene::Scene(QJsonObject & data)
{
    getIntOrFail(id, data, "Scene", "ID");
    getQStringOrFail(name, data, "Scene", "Name");
}

QJsonObject Scene::exportAsJson()
{
    QJsonObject jTile;
    jTile["ID"] = id;
    jTile["Name"] = name;

    return jTile;
}
