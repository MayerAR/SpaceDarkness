#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "gameWidget/objects/object.h"


class stdWeaponClip;

class GameObject: public Object
{
public:
    GameObject(const QString &pixmapPath, const QSize &size, objectID curID_, strategyID strID_, objectID curBullet_);
    ~GameObject() override;

    objectID idBullet() const;

    void changeBullet(objectID idB);
    void resetHealth(); //set health on base value
    bool changeHealth(int deltaHealth);
    int currentHealth() const;
    bool ifAlive() const override; //if health < 0 return false
    bool couldShot() const;
    void fixShot();
    void resetShot();
protected:
    void doUpdate(const std::shared_ptr<ChangeObjectData> upData) override;

private:

    objectID curBullet;
    int health;
    std::unique_ptr<stdWeaponClip> curWeapon;
};

#endif // GAMEOBJECT_H
