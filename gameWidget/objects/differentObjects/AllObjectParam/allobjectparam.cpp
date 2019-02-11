#include "allobjectparam.h"
#include "gameWidget/loadStage/loadStage.h"

AllObjectParam* AllObjectParam::curP = nullptr;

AllObjectParam::AllObjectParam()
{
    param = createObject::getSingle()->createAllObjectParam();
}

AllObjectParam::~AllObjectParam()
{
    delete param;
}


const AllObjectParam *AllObjectParam::getSingle()
{
    if(curP == nullptr)
        curP = new AllObjectParam();
    return curP;
}

void AllObjectParam::deleteParam()
{
    delete curP;
    curP = nullptr;
}



const std::shared_ptr<ObjectParam> AllObjectParam::currentParam(objectID id) const
{
    if(param->find(id) == param->end())
        id = objectID::none;

    return (*param)[id];
}



