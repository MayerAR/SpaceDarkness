#ifndef INTERFACESTRATEGY_H
#define INTERFACESTRATEGY_H

#include <memory>
#include "gameWidget/constants.h"

struct ChangeObjectData;
class Object;


//interface of object's strategy
class AbstractStrategy
{
public:
    static AbstractStrategy* createStrategy(strategyID id, const Object *newItem);
    virtual ~AbstractStrategy();

    //return data about current update curObject
    virtual const std::shared_ptr<ChangeObjectData> nextStep() = 0;
    //add Impulse to current item
    virtual void applyImpulse(const Vector2D&, const Vector2D&) = 0;
    virtual void setBumbReaction(bool b) = 0;
    //try to avaid bumb with object
    virtual void avaidBumbWithObject(const std::shared_ptr<Object>) = 0;
    virtual void update() = 0;
    virtual strategyID idStrategy() const = 0;

};

#endif // INTERFACESTRATEGY_H
