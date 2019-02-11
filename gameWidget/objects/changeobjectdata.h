#ifndef CHANGEOBJECTDATA_H
#define CHANGEOBJECTDATA_H

#include "gameWidget/constants.h"

struct ChangeObjectData
{
    QPointF deltaPosition = QPointF(0, 0);
    Vector2D deltaVelocity = Vector2D(0, 0);
    qreal deltaZVelocity = 0;
    qreal angleRad = 0;
    bool ifShot = false;

    ChangeObjectData(){}

    void resetData();
    void setDeltaPos(const QPointF& delta);
    void setDeltaVelocity(Vector2D velocity);
    void setDeltaZVelocity(qreal velocity);
    void setAngleRad(qreal angle);
    void setIfShot(bool ifShot_);
private:
    ChangeObjectData(const ChangeObjectData&) = delete;
    ChangeObjectData& operator=(const ChangeObjectData&) = delete;
};

#endif // CHANGEOBJECTDATA_H















