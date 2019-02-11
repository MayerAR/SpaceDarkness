#ifndef FIRSTSHIPSTRATEGY_H
#define FIRSTSHIPSTRATEGY_H

#include "gameWidget/strategy/objectStrategy/basestrategy.h"
#include <QList>

class Object;
class LocalVelocity;
class Line2D;


class FirstShipStrategy: public BaseShipStrategy
{
public:
    FirstShipStrategy(const Object* curItem_, qreal dist = 150);
    ~FirstShipStrategy() override;
    void setDistance(qreal dist);


    const std::shared_ptr<ChangeObjectData> nextStep() override;
    void avaidBumbWithObject(const std::shared_ptr<Object> otherItem) override;

protected:
    bool shouldDoShot() const override;

private:

    Vector2D bumbWithObject(const std::shared_ptr<Object> otherItem) const;
    std::unique_ptr<Line2D> findLineIntersection(const std::shared_ptr<Object> otherItem) const;
    Vector2D avaidObjectVelocity();
    virtual Vector2D attackTargetVelocity(std::shared_ptr<Object> target);
    virtual qreal attackTargetAngle(std::shared_ptr<Object> target);
    //    EdgeVelocity attakCalculate(const std::shared_ptr<Object> & otherItem) const;
//    qreal rotateCalculate(const std::shared_ptr<Object> & otherItem) const;



//    Border findBorderByPoint(const QPointF &pp, const QRectF &rect) const;

    qreal normDistanceSquare;
    QList<Vector2D> avaidObjects;
};



#endif // FIRSTSHIPSTRATEGY_H
