#ifndef BASESTRATEGY_H
#define BASESTRATEGY_H

#include "gameWidget/strategy/InterfaceStrategy.h"

//this strategy is need for move by line
//need for bullet and rock
class BaseStrategy: public AbstractStrategy
{
//methods
public:
    BaseStrategy(const Object *curItem_);
    ~BaseStrategy() override;//we mustn't delete curItem, because  curItem delete this object!

    // AbstractStrategy interface
    const std::shared_ptr<ChangeObjectData> nextStep() override;
    void applyImpulse(const Vector2D &j, const Vector2D &r) override;
    void setBumbReaction(bool b) override;
    void avaidBumbWithObject(const std::shared_ptr<Object> ) override;
    void update() override;
    strategyID idStrategy() const override;

protected:
    const Object* currentItem() const;
    bool getBumbReaction() const;

//data
private:
    const Object* curItem; //need for data of curStrategy's object
    bool bumbReaction;
protected:
    std::shared_ptr<ChangeObjectData> objData; // all child class must change this variable
};


//this strategy will used for for mainShip
class BaseShipStrategy: public BaseStrategy
{
public:
    BaseShipStrategy(const Object *curItem_);
    ~BaseShipStrategy() override {}
    const std::shared_ptr<ChangeObjectData> nextStep() override;
    void applyImpulse(const Vector2D &j, const Vector2D &r) override;
    strategyID idStrategy() const override;

protected:
    virtual void bumbTrajectory();
    virtual bool shouldDoShot() const;


//    std::shared_ptr<stdWeaponClip> curWeapon;
    qreal ticksOfBumb; //how long will our element bounce off
    qreal wDelta;

};

#endif // BASESTRATEGY_H
