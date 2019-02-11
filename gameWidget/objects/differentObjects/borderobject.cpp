#include "borderobject.h"
#include <QDebug>

BorderObject::BorderObject(const QString &pixmapPath, const QSize &size, objectID curID_, strategyID strID_):
    Object(pixmapPath, size, curID_, strID_)
{
}

BorderObject::~BorderObject()
{
}

bool BorderObject::ifAlive() const
{
    return true;
}

void BorderObject::doUpdate(const std::shared_ptr<ChangeObjectData> /*upData*/)
{
    qDebug()<<"It's not normal!: BorderObject::doUpdate(...)";
    system("pause");
}
