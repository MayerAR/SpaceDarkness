#ifndef BULLETOBJECT_H
#define BULLETOBJECT_H

#include "gameWidget/objects/object.h"


//class that couldn't change direction
class BulletObject: public Object// we save this in other vector
{
public:
    BulletObject(const QString &pixmapPath, const QSize &size, objectID curID_,strategyID strID_);
    ~BulletObject() override;

    bool ifAlive() const override;
    objectID idBullet() const;

    void setDamage(int newDamage);
    int currentDamage() const;
private:
    int damage;
};


#endif // BULLETOBJECT_H
