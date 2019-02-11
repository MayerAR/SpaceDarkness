#include "weaponparam.h"
#include "gameWidget/loadStage/loadStage.h"
#include "gameWidget/constants.h"
#include <QDebug>

AllWeaponParam* AllWeaponParam::curP = nullptr;
//weapon Param
AllWeaponParam::AllWeaponParam()
{
    weapons = createObject::getSingle()->createAllWeaponParam();
}

AllWeaponParam::~AllWeaponParam()
{
    delete weapons;
}

const AllWeaponParam *AllWeaponParam::getSingle()
{
    if(curP == nullptr)
        curP = new AllWeaponParam;
    return curP;
}

void AllWeaponParam::deleteParam()
{
    delete curP;
    curP = nullptr;
}


std::shared_ptr<WeaponParam> AllWeaponParam::currentParam(objectID id) const
{

    if(weapons->find(id) == weapons->end())
        id = objectID::none;

    return (*weapons)[id];
}

stdWeaponClip::stdWeaponClip(const objectID &currentID):
    curID(currentID),
    curParam(AllWeaponParam::getSingle()->currentParam(currentID)),
    bulletsInClip(curParam->clip),
    ticksOfReload(0),
    numbRecoil(0)
{}

void stdWeaponClip::decreaseTicks()
{
    if(ticksOfReload > 0)
        ticksOfReload--;
}

bool stdWeaponClip::couldShoot(objectID id)
{
    if(curID != id)
        changeBullet(id);

    if(ticksOfReload > 1) //from 0 to 1 is a space for recoil
        return false;

    return true;
}

bool stdWeaponClip::fixShot()
{
    bool ifRecoil(true);
    if(ticksOfReload <= 0)
    {
        numbRecoil = 0;
        ifRecoil = false;
    }
    numbRecoil = numbRecoil + 1; // with new shoot our recoil are more and more stronger

    bulletsInClip--;

    if(bulletsInClip == 0)
        reload(true);  //ticksOfReload is changed
    else
        reload(false); //ticksOfReload is changed

    return ifRecoil;
}

void stdWeaponClip::resetWeapon()
{
    numbRecoil = 0;
}

qreal stdWeaponClip::recoilImpulse()
{
    return numbRecoil * curParam->recoil;
}

void stdWeaponClip::changeBullet(objectID newID)
{
    curParam.reset();
    curParam = AllWeaponParam::getSingle()->currentParam(newID); //change weapon param
    reload(true); //set long reload
}

void stdWeaponClip::reload(bool ifLongReload)
{
    if(ifLongReload)
    {
        ticksOfReload = curParam->reloadTicks;
        bulletsInClip = curParam->clip;
        numbRecoil = 0;
    }
    else
        ticksOfReload = curParam->betweenShotsTicks;
}
