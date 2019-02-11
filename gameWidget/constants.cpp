#include "constants.h"
#include <QRectF>
#include <QLineF>
#include <QDebug>
#include <cmath>


qreal V2Extend::PerpDot(const Vector2D &self, const Vector2D &vector)
{
    return self.x() * vector.y() - self.y() * vector.x();
}

Vector2D V2Extend::Perp(const Vector2D &self)
{
    return Vector2D(-self.y(), self.x());
}

qreal V2Extend::Dot(const Vector2D &self, const Vector2D &vector)
{
    return self.x()*vector.x() + self.y()*vector.y();
}

Vector2D V2Extend::Dot(const Vector2D &self, qreal k)
{
    return Vector2D(self.x() * k, self.y() * k);
}

Vector2D V2Extend::Dot(qreal k, const Vector2D& self)
{
    return V2Extend::Dot(self, k);
}


Vector2D V2Extend::Negative(const Vector2D &self)
{
    return Vector2D(-self.x(), -self.y());
}

Vector2D V2Extend::Rotate(const Vector2D &self, const Vector2D& vector)
{
    return Vector2D(self.x()*vector.x() - self.y()*vector.y(), self.x()*vector.y() + self.y()*vector.x());
}

Vector2D V2Extend::RotateCoordinate(const Vector2D& vect, qreal angleRad)
{
    Vector2D resVelocity;
    resVelocity.setX(vect.x() * std::cos(angleRad) - vect.y() * std::sin(angleRad)); //rotate from scene coordinate system to local coordinate system
    resVelocity.setY(vect.y() * std::cos(angleRad) + vect.x()* std::sin(angleRad));
    return resVelocity;
}

Vector2D V2Extend::Normalize(const Vector2D &self)
{
    if(self.x() == 0 && self.y() == 0)
        return Vector2D(0, 0);

    Vector2D vector = self;
    qreal inv_length = 1 / std::sqrt(self.x() * self.x() + self.y() * self.y());
    vector.setX(self.x() * inv_length);
    vector.setY(self.y() * inv_length);
    return vector;
}

int randInt(int first, int second)
{
    return (std::rand()%(second - first)) + first;
}

qreal normalizeAngle(qreal angle)
{
    while(angle < 0)
        angle += d_pi;
    while(angle > d_pi)
        angle -= d_pi;
    return angle;
}
//return angle in radian
qreal calculateAngle(const QPointF &centerOfItem, const QPointF &target)
{
    QLineF lineToTarget(centerOfItem, target);
    if(lineToTarget.length() == 0) //centerOfItem = target
        return 0.0;

    qreal angleToTarget = acos(lineToTarget.dx()/lineToTarget.length());

    if(lineToTarget.dy() < 0)
        angleToTarget = d_pi - angleToTarget;
    angleToTarget = (pi - angleToTarget) + pi/2;

    if(angleToTarget > pi)
        angleToTarget -= d_pi;


    return -angleToTarget;
}

QString nameOfObject(const objectID& id)
{
    switch(id)
    {
    case objectID::none:
        return "none";
    case objectID::stdBullet:
        return "stdBullet";
    case objectID::Rock:
        return "Rock";
    case objectID::stdShip:
        return "stdShip";
    default:
        qDebug()<<"Error can't find objectID: "<<static_cast<int>(id)<<"in nameOfBullet(bulletID id)";
    }
    return "";
}

objectID idOfObject(const QString& name)
{
    if(name == "stdBullet")
        return objectID::stdBullet;

    if(name == "Rock")
        return objectID::Rock;

    if(name == "stdShip")
        return objectID::stdShip;

    qDebug()<<"Error can't find bullet Name: "<<name<<"in idOfBullet(bulletID id)";

    return objectID::none;
}

QString nameOfStrategy(const strategyID &id)
{
    switch(id)
    {
    case strategyID::none:
        return "none";

    case strategyID::BaseStrategy:
        return "BaseStrategy";
    case strategyID::BaseShipStrategy:
        return "BaseShipStrategy";
    case strategyID::FirstShipStrategy:
        return "FirstShipStrategy";
    default:
        qDebug()<<"Error can't find strategyID: "<<static_cast<int>(id)<<"in nameOfStrategy(...)";
    }
    return "none";
}

strategyID idOfStrategy(const QString &name)
{
    if(name == "none")
        return strategyID::none;

    if(name == "FirstShipStrategy")
        return strategyID::FirstShipStrategy;

    if(name == "BaseStrategy")
        return strategyID::BaseStrategy;
    if(name == "BaseShipStrategy")
        return strategyID::BaseShipStrategy;


    qDebug()<<"Error can't find strategy Name: "<<name<<"in idOfStrategy(...)";
    return strategyID::none;

}







