#ifndef WEAPONPARAM_H
#define WEAPONPARAM_H

#include <memory>
#include <QMap>
#include "gameWidget/constants.h"

//save param of one type of weapon
struct WeaponParam //need for save param of current weapon( not bullet, but weapon)
{
    int clip = 0;
    int betweenShotsTicks = 0;
    int reloadTicks = 0;
    int recoil = 0;
};

//save param of all types of weapon
//singleton
class AllWeaponParam
{
private:
    static AllWeaponParam* curP;
    QMap<objectID, std::shared_ptr<WeaponParam>>* weapons;

    AllWeaponParam();
    AllWeaponParam(const AllWeaponParam&) = delete; //this is a single
    AllWeaponParam& operator=(const AllWeaponParam&) = delete; //this is a single
public:
    ~AllWeaponParam();
    static const AllWeaponParam* getSingle(); //create and/or return single object
    static void deleteParam();
    std::shared_ptr<WeaponParam> currentParam(objectID id) const; //return current shared_pointer ( param weapon with bulletID = id)

};

//standart weapon Clip
class stdWeaponClip
{
private:
    objectID curID;
    std::shared_ptr<WeaponParam> curParam;
    int bulletsInClip;
    int ticksOfReload;
    int numbRecoil;

    stdWeaponClip(const stdWeaponClip&) = delete;
    stdWeaponClip& operator=(const stdWeaponClip&) = delete;
public:
    stdWeaponClip(const objectID& currentID); //save param of weapon in curParam
    void decreaseTicks();
    bool couldShoot(objectID id);
    //return true if bullet create with recoil
    bool fixShot(); //this function like a signal -- bullet is created
    void resetWeapon();
    qreal recoilImpulse();
    void changeBullet(objectID newID);

    void reload(bool ifLongReload); //reload current clip

};

#endif // WEAPONPARAM_H
