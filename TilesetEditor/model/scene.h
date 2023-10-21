#ifndef SCENE_H
#define SCENE_H

#include <QJsonObject>
#include <QString>

class Scene
{
public:
    int id;
    QString name;

public:
    Scene();
    Scene(QJsonObject & data);
    QJsonObject exportAsJson();

};

#endif // SCENE_H
