#ifndef CLUSTER_H
#define CLUSTER_H

#include <QJsonObject>
#include <QString>

class Cluster
{
public:
    int id;
    QString name;

public:
    Cluster();
    Cluster(QJsonObject & data);
    QJsonObject exportAsJson();

};

#endif // CLUSTER_H
