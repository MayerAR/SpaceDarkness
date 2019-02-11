#ifndef ALLOBJECTPARAM_H
#define ALLOBJECTPARAM_H

#include <memory>
#include <QMap>
#include "gameWidget/constants.h"


//for more easy need for save information about current object
struct ObjectParam
{
    qreal acceleration;
    qreal limitVelocity;
    qreal limitRotation;
    int rotateAcceleration;
    bool isStatic;
    qreal elasticity;
    qreal mass;
    qreal inv_mass; //1 / mass
    int health;
};


//save const of all objects
class AllObjectParam
{
private:
    static AllObjectParam* curP;
    QMap<objectID, std::shared_ptr<ObjectParam>>* param;

    AllObjectParam();
    AllObjectParam(const AllObjectParam&) = delete;
    AllObjectParam& operator=(const AllObjectParam&) = delete;
    ~AllObjectParam();
public:
    static const AllObjectParam* getSingle();
    static void deleteParam();
    const std::shared_ptr<ObjectParam> currentParam(objectID id) const;
};

#endif // ALLOBJECTPARAM_H
