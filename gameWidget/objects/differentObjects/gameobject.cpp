#include "gameobject.h"
#include <QGraphicsPixmapItem>
#include "gameWidget/strategy/InterfaceStrategy.h"
#include "gameWidget/objects/differentObjects/WeaponParam/weaponparam.h"
#include "gameWidget/objects/differentObjects/AllObjectParam/allobjectparam.h"
#include "gameWidget/animationItem/animationItem.h"

GameObject::GameObject(const QString &pixmapPath, const QSize &size, objectID curID_, strategyID strID_, objectID curBullet_):
    Object(pixmapPath, size, curID_, strID_),
    curBullet(curBullet_),
    curWeapon(new stdWeaponClip(curBullet))
{
    resetHealth();
}

GameObject::~GameObject()
{

}

objectID GameObject::idBullet() const
{
    return curBullet;
}

void GameObject::changeBullet(objectID idB)
{
    if(curBullet == idB)
        return;

    curBullet = idB;
    curWeapon->changeBullet(curBullet);
}

void GameObject::resetHealth()
{
    health = AllObjectParam::getSingle()->
            currentParam(idObject())->health;
}

bool GameObject::changeHealth(int deltaHealth)
{
    health += deltaHealth;

    //create animation
    auto animationItem = new TextAnimation(QString::number(deltaHealth), pixmapItem().get());
    animationItem->setPos(0, -pixmapItem()->pixmap().rect().height());
    animationItem->start();

    return ifAlive();
}

int GameObject::currentHealth() const
{
    return health;
}

bool GameObject::ifAlive() const
{
    return health > 0;
}

bool GameObject::couldShot() const
{
    return curWeapon->couldShoot(idBullet());
}

void GameObject::fixShot()
{
    if(curWeapon->fixShot()) //if we have recoil --> if(true)
    {
        Vector2D j(0, curWeapon->recoilImpulse());

        j = V2Extend::RotateCoordinate(j, rotationAngleRad());

        qreal h = pixmapItem()->pixmap().height()>>1;
        int w = pixmapItem()->pixmap().width();
        Vector2D r (static_cast<qreal>(randInt(0, w<<1) - w),
                    -h);
        currentStrategy()->applyImpulse(j, r);
    }

}

void GameObject::resetShot()
{
    curWeapon->resetWeapon();
}

void GameObject::doUpdate(const std::shared_ptr<ChangeObjectData> upData)
{
    curWeapon->decreaseTicks();
    Object::doUpdate(upData);

}
