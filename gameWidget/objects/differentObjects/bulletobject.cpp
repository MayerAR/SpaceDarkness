#include "bulletobject.h"
#include <QGraphicsItem>
#include <QGraphicsScene>

BulletObject::BulletObject(const QString &pixmapPath, const QSize &size, objectID curID_, strategyID strID_):
    Object(pixmapPath, size, curID_, strID_),
    damage(0)
{}

BulletObject::~BulletObject()
{

}

objectID BulletObject::idBullet() const
{
    return idObject();
}

void BulletObject::setDamage(int newDamage)
{
    damage = newDamage;
}

int BulletObject::currentDamage() const
{
    return damage;
}

bool BulletObject::ifAlive() const
{
    QPointF posBullet = pixmapItem()->pos();
    //if bullet is beyond the boundaries of the scene
    if(posBullet.x() < 0
            || posBullet.y() < 0
            || posBullet.x() > pixmapItem()->scene()->width()
            || posBullet.y() > pixmapItem()->scene()->height())
    {
        return false;
    }

    return true;
}

