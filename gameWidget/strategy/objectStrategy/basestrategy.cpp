#include "basestrategy.h"
#include "gameWidget/objects/object.h"
#include "gameWidget/objects/changeobjectdata.h"
#include "gameWidget/objects/differentObjects/AllObjectParam/allobjectparam.h"
#include <QGraphicsItem>
#include <QDebug>

BaseStrategy::BaseStrategy(const Object *curItem_):
    curItem(curItem_),
    bumbReaction(false),
    objData(new ChangeObjectData())
{}

BaseStrategy::~BaseStrategy() {}

const std::shared_ptr<ChangeObjectData> BaseStrategy::nextStep()
{
    objData->setDeltaPos(curItem->curVelocity() * timeStamp);
    objData->setAngleRad(curItem->rotationVelocity() * timeStamp);
    //we don't change velocity and we don't change ifShot, because current object dont have weapon!
    return objData;
}


void BaseStrategy::applyImpulse(const Vector2D& j, const Vector2D& r)
{
    if(curItem->isStatic())
        return;

    qreal velocityFactor = 2; //0.5
    qreal rotationFactor = 3;//0.2


    objData->setDeltaVelocity(velocityFactor * j * curItem->inv_mass());
    objData->setDeltaZVelocity(rotationFactor * V2Extend::PerpDot(r, j) * curItem->invImpulse());
}

void BaseStrategy::setBumbReaction(bool b)
{
    bumbReaction = b;
}

void BaseStrategy::avaidBumbWithObject(const std::shared_ptr<Object>)
{
    //this strategy dont try to avaid other objects
    return;
}

void BaseStrategy::update()
{
    objData->resetData();
}

strategyID BaseStrategy::idStrategy() const
{
    return strategyID::BaseStrategy;
}

const Object *BaseStrategy::currentItem() const
{
    return curItem;
}

bool BaseStrategy::getBumbReaction() const
{
    return bumbReaction;
}



//BaseShipStrategy

BaseShipStrategy::BaseShipStrategy(const Object *curItem_):
    BaseStrategy(curItem_),
    ticksOfBumb(0),
    wDelta(0)
{
}

const std::shared_ptr<ChangeObjectData> BaseShipStrategy::nextStep()
{
    if(ticksOfBumb > 0)
        bumbTrajectory();

    objData->setIfShot(shouldDoShot());

    return BaseStrategy::nextStep();
}

void BaseShipStrategy::applyImpulse(const Vector2D &j, const Vector2D &r)
{
    BaseStrategy::applyImpulse(j, r);

    qreal w = currentItem()->rotationVelocity() + objData->deltaZVelocity;

    auto limitRotation = AllObjectParam::getSingle()->currentParam(currentItem()->idObject())->limitRotation;
    if(w < -limitRotation)
        w = -limitRotation;
    else if (w > limitRotation)
        w = limitRotation;

    ticksOfBumb = std::abs(w) * 40;
    wDelta = -w / ticksOfBumb;
}

strategyID BaseShipStrategy::idStrategy() const
{
    return strategyID::BaseShipStrategy;
}

void BaseShipStrategy::bumbTrajectory()
{
    ticksOfBumb--;
    if(ticksOfBumb <= 0)
    {
        wDelta = 0;
        objData->setDeltaZVelocity(-currentItem()->rotationVelocity()); //set current velocity = 0;
        return;
    }

    objData->setDeltaZVelocity(objData->deltaZVelocity + wDelta);
}

bool BaseShipStrategy::shouldDoShot() const
{
    return false;
}








/* Base Ship Strategy
 *      will create for mainShip
 */

//BaseShipStrategy::BaseShipStrategy(const Object *curItem_): GameStrategy(curItem_)
//{
//    stdWeaponClip param = stdWeaponClip(currentItem()->idBullet());
//    curWeapon = std::make_shared<stdWeaponClip>(param);

//    ticksOfBumb = 0;
//}

//QVector3D BaseShipStrategy::nextStep()
//{
//    curWeapon->decreaseTicks();
//    if(ifBumb())
//        return bumbTrajectory();

//    return GameStrategy::deltaStep();
//}

//bool BaseShipStrategy::ifBumb() const
//{
//    return ticksOfBumb > 0;
//}
////r - local coordinate
//void BaseShipStrategy::ApplyImpulse(const QVector2D &j, const QVector2D &r)
//{
//    GameStrategy::ApplyImpulse(j, r);
//    ticksOfBumb = std::abs(static_cast<int>(currentItem()->w))>>2;
//    wDecrease = currentItem()->w/ticksOfBumb;
//}

//bool BaseShipStrategy::doShot()
//{
//    if(!curWeapon->couldShoot(currentItem()->idBullet()))
//        return false;

//    if(!shouldWeDoShot())
//        return false;

//    if(curWeapon->fixShoot())
//    {
//        if(!currentItem()->isUpdate)
//            Contact::updateSceneEdge(currentItem());

//        QVector2D impulse = V2Extend::Dot(-static_cast<float>(
//                                              curWeapon->recoilImpulse()),
//                                          currentItem()->ed[1].n);

//        int width = static_cast<int>(currentItem()->pixmapItem->pixmapRect().width());
//        int i = randInt(0, 2*width) - (width); //set center of impulse
//        QVector2D r(i, 0);
//        this->ApplyImpulse(impulse, r);
//    }
//    return true;
//}

//QVector3D BaseShipStrategy::bumbTrajectory()
//{

//    currentItem()->w -= wDecrease;
//    ticksOfBumb--;
//    if(ticksOfBumb == 0)
//    {
//        currentItem()->w = 0;
//    }

//    return GameStrategy::deltaStep();
//}

//bool BaseShipStrategy::shouldWeDoShot()
//{
//    return true;
//}



