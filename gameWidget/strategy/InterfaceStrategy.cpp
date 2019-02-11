#include "gameWidget/strategy/InterfaceStrategy.h"
#include "gameWidget/strategy/objectStrategy/basestrategy.h"
#include "gameWidget/strategy/objectStrategy/firstshipstrategy.h"

AbstractStrategy *AbstractStrategy::createStrategy(strategyID id, const Object *newItem)
{
    switch(id)
    {
    case strategyID::BaseStrategy:
        return new BaseStrategy(newItem);
        break;
    case strategyID::BaseShipStrategy:
        return new BaseShipStrategy(newItem);
        break;
    case strategyID::FirstShipStrategy:
        return new FirstShipStrategy(newItem);
        break;
    default:
        return nullptr;
    }
}

AbstractStrategy::~AbstractStrategy()
{

}
