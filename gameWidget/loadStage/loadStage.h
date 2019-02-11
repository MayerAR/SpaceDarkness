#ifndef LOADSTAGE_H
#define LOADSTAGE_H

#include "gameWidget/constants.h"
#include <memory>
struct xmlElem;
class GameObject;
class BulletObject;
class QGraphicsItem;
class EndingAnimation;

struct WeaponParam;
struct ObjectParam;

//load objects of cur wave
class LoadWave
{
public:
    LoadWave(const std::shared_ptr<xmlElem>& waves_);
    ~LoadWave();
    QList<std::shared_ptr<GameObject> > nextWave();
    bool ifNextWave() const;
    void restartWaves();
private:

    int numbWave;//numb of current wave
    int limitWave;//numb of all waves
    std::shared_ptr<xmlElem> waves; //save all type of ships and rocks in xml format
    //waves -- all waves of current stage
};

class createObject
{
private:
    static createObject* p_single;
    std::shared_ptr<xmlElem> curParam;
    std::shared_ptr<xmlElem> bulletModel;
    std::shared_ptr<xmlElem> shipModel;
    std::shared_ptr<xmlElem> animation;
    std::shared_ptr<xmlElem> strategy;
    qreal scale;

    std::shared_ptr<ObjectParam> paramOfItem(std::shared_ptr<xmlElem> param) const;
    std::shared_ptr<ObjectParam> paramOfBullet(std::shared_ptr<xmlElem> param) const;
    std::shared_ptr<ObjectParam> paramOfBorder(std::shared_ptr<xmlElem> param) const;
    std::shared_ptr<GameObject> createRock(const QString& name, std::shared_ptr<xmlElem> param) const;
    std::shared_ptr<WeaponParam> paramOfWeapon(std::shared_ptr<xmlElem> param) const;
    createObject();
    ~createObject();
public:
    void deleteSingle();
    void setScale(const qreal& scale_);
    //create item --> name == type of item, param -- stage parametres of current item[ not base parametres]


    std::shared_ptr<GameObject> createItem(const QString& name, std::shared_ptr<xmlElem> param) const;
    std::shared_ptr<GameObject> createMainItem() const;

    //in ship we save paramtres about current bullet
    std::shared_ptr<BulletObject> createBullet(const std::shared_ptr<GameObject> ship) const;


    //animation from bullet
    EndingAnimation* createAnimationExplosionBullet(objectID id, QGraphicsItem *parent = nullptr) const;
    QMap<objectID, std::shared_ptr<WeaponParam>> *createAllWeaponParam() const;
    QMap<objectID, std::shared_ptr<ObjectParam>>* createAllObjectParam() const;
    //singleton
    static createObject* getSingle()
    {
        if(!p_single)
            p_single = new createObject;
        return p_single;
    }
};

#endif // LOADSTAGE_H
