#include "changeobjectdata.h"

void ChangeObjectData::resetData()
{
    deltaPosition = QPointF(0, 0);
    deltaVelocity = Vector2D(0, 0);
    deltaZVelocity = 0;
    angleRad = 0;
    ifShot = false;
}

void ChangeObjectData::setDeltaPos(const QPointF &delta)
{
    deltaPosition = delta;
}

void ChangeObjectData::setDeltaZVelocity(qreal velocity)
{
    deltaZVelocity = velocity;
}

void ChangeObjectData::setDeltaVelocity(Vector2D velocity)
{
    deltaVelocity = velocity;
}

void ChangeObjectData::setAngleRad(qreal angle)
{
    angleRad = angle;
}

void ChangeObjectData::setIfShot(bool ifShot_)
{
    ifShot = ifShot_;
}

