#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QPointF>

using Vector2D = QPointF; //i want use QPointF with name Vector, because variables with this type will be vectors

const qreal pi = 3.14159265358;
const qreal d_pi = 6.28318530718;
const qreal fromRadToDegrees = 180/pi;
const qreal fromDegreesToRad = pi/180;

enum class strategyID{none = 0, BaseStrategy = 1, BaseShipStrategy = 2, FirstShipStrategy = 3, first = 1, last = 3};
enum class objectID{none = 0, stdBullet = 1, Rock = 2, stdShip = 3, Border = 4, first = 1, last = 4};

const qreal timeStamp = 0.01;
//const int curTargetStrategy_LengthToTarget = 100;



//it's not constants
class QString;

#include <qglobal.h>
#include <QVector2D>


class V2Extend
{
public:
    static qreal PerpDot(const Vector2D& self, const Vector2D& vector);
    static Vector2D Perp(const Vector2D& self);
    static qreal Dot(const Vector2D& self, const Vector2D& vector);
    static Vector2D Dot(const Vector2D& self, qreal k);
    static Vector2D Dot(qreal k, const Vector2D& self);
    static Vector2D Negative(const Vector2D& self);
    static Vector2D Rotate(const Vector2D& self, const Vector2D &vector);
    static Vector2D RotateCoordinate(const Vector2D &vect, qreal angleRad);
    static Vector2D Normalize(const Vector2D& self);
};

int randInt(int first, int second);

qreal normalizeAngle(qreal angle);

//retur angle in radian
qreal calculateAngle(const QPointF &centerOfItem, const QPointF &target);

QString nameOfObject(const objectID& id);
objectID idOfObject(const QString& name);

QString nameOfStrategy(const strategyID& id);
strategyID idOfStrategy(const QString& name);

#endif // CONSTANTS_H
