#include "firstshipstrategy.h"
//#include "strategy/objectStrategy/avaidClasses/LocalVelocity.h"
#include "gameWidget/objects/allobjects.h"
#include "gameWidget/objects/differentObjects/pixmapParametres/pixmapitem.h"
#include "gameWidget/strategy/objectStrategy/avaidClasses/line2d.h"
#include "gameWidget/objects/changeobjectdata.h"
#include "gameWidget/objects/differentObjects/AllObjectParam/allobjectparam.h"
#include "gameWidget/mainItem/mainItem.h"



FirstShipStrategy::FirstShipStrategy(const Object *curItem_, qreal dist):
    BaseShipStrategy(curItem_),
    normDistanceSquare(dist * dist)
{}

FirstShipStrategy::~FirstShipStrategy()
{

}


void FirstShipStrategy::setDistance(qreal dist)
{
    normDistanceSquare = dist * dist;
}


const std::shared_ptr<ChangeObjectData> FirstShipStrategy::nextStep()
{
    if(ticksOfBumb <= 0) //it's not good, because in BaseShipStrategy::nextStep() we check ticksOfBumb again....
    {
        if(avaidObjects.size() != 0)
            objData->setDeltaVelocity(avaidObjectVelocity());
        else
            objData->setDeltaVelocity(attackTargetVelocity(MainItem::getMainItem()->getObject()));
    }
    qreal angleRad = attackTargetAngle(MainItem::getMainItem()->getObject());

    if(std::abs(angleRad) > 0.001)
    {
        angleRad /= 2;
        auto rotateAcceleration = AllObjectParam::getSingle()->currentParam(currentItem()->idObject())->rotateAcceleration;
        if(std::abs(angleRad) < 1)
            objData->setDeltaZVelocity(rotateAcceleration * angleRad);
        else
             objData->setDeltaZVelocity(rotateAcceleration * (angleRad > 0 ? 1 : -1));
    }




    return BaseShipStrategy::nextStep();
}

void FirstShipStrategy::avaidBumbWithObject(const std::shared_ptr<Object> otherItem)
{
    auto velocity = bumbWithObject(otherItem);
    if(velocity != Vector2D(0, 0))
        avaidObjects.push_back(velocity);
}

bool FirstShipStrategy::shouldDoShot() const
{
    if(objData->deltaZVelocity == 0)
    {
        auto targetPosition = MainItem::getMainItem()->getObject()->pixmapItem()->mapToItem(
                                                    currentItem()->pixmapItem().get(), QPointF(0, 0));
        auto distance = targetPosition.x() * targetPosition.x() + targetPosition.y() * targetPosition.y();
        if(distance < 2 * normDistanceSquare)
            return true;
    }
    return false;
}

Vector2D FirstShipStrategy::bumbWithObject(const std::shared_ptr<Object> otherItem) const
{

    //find bullet trajectory that intersection currentItem
    std::unique_ptr<Line2D> line(findLineIntersection(otherItem));
    if(line == nullptr) //if don't find
        return Vector2D(0, 0);

    auto points = line->intersected(currentItem()->pixmapItem()->boundingRect()); //find points
    auto centerOfOtherItem = otherItem->pixmapItem()->mapToItem(currentItem()->pixmapItem().get(), QPointF(0, 0));//translate center point of otherItem (0, 0) to current item's local coordinate system
    //and point in points(QVector) are sort by this rule (point on LeftBorder < point on TopBorder < RightBord < BottomBorder)

    //claculate secondVelocity. it's a vector from otherItem to current item
    Vector2D firstVelocity = V2Extend::Negative(
                                    V2Extend::Normalize(
                                            Vector2D(centerOfOtherItem)));

    //calculate secondVelocity
    Vector2D secondVelocity(0, 0);
    if(!points.empty())
    {
        secondVelocity = V2Extend::Normalize(
                            V2Extend::Perp(
                                Vector2D(points[0] - centerOfOtherItem)));
        Vector2D vectToCenter(centerOfOtherItem); // vect from (0, 0) [ zero of currentItem coordinate system] to centerOfOtherItem
        if(V2Extend::Dot(secondVelocity, vectToCenter) > 0) //wrong! we must set negative to the firstVelocity
            secondVelocity = V2Extend::Negative(secondVelocity);
    }



    firstVelocity = V2Extend::Normalize(0.5 * firstVelocity + secondVelocity); //prepear result
    return firstVelocity;
}

//find line between moving otherItem and not moving curItem [ we take velocity from item and add it to other velocity]
// calculate borders of otherItem's pixmapRect and answer the question: If our otherItem is shot current item? and what line is that?
std::unique_ptr<Line2D> FirstShipStrategy::findLineIntersection(const std::shared_ptr<Object> otherItem) const
{
    //we want to go to the curItem's coorinate system -- in this system our item has got velocity = 0, 0;

    QRectF pRect(currentItem()->pixmapItem()->boundingRect());
    auto otherVelocity = otherItem->curVelocity();
    auto curVelocity = currentItem()->curVelocity();
    //this is velocity of otherItem in current item coordinate system (and velocity of current item in current item coordinate system is 0)
    Vector2D velGlobal = Vector2D(otherVelocity.x() - curVelocity.x(),
                                     otherVelocity.y() - curVelocity.y());  //curItem's velocity = 0


    Vector2D bulletVelocity = V2Extend::RotateCoordinate(velGlobal, currentItem()->rotationAngleRad());
//    bulletVelocity.setX(velGlobal.x() * std::cos(rad) + velGlobal.y() * std::sin(rad)); //rotate from scene coordinate system to local coordinate system
//    bulletVelocity.setY(velGlobal.y() * std::cos(rad) - velGlobal.x()* std::sin(rad));


    auto localOtherRect = otherItem->localPixmapRect(); //other item's rect in other item's local coordinate system

    //create vector with corners in otherItem's coordinate system [for more comfortable find]
    QVector<QPointF> corners{localOtherRect.center(),
                                localOtherRect.topLeft() + QPointF(-3, -3),
                                localOtherRect.topRight() + QPointF(3, -3),
                                localOtherRect.bottomRight() + QPointF(3, 3),
                                localOtherRect.bottomLeft() + QPointF(-3, 3)};

    const QPointF delta = 0.5 * bulletVelocity;

    std::unique_ptr<Line2D> line(new Line2D()); //create unique
    for(auto& begPoint: corners)
    {
        begPoint = otherItem->pixmapItem()->mapToItem(currentItem()->pixmapItem().get(), begPoint); //from otherItem's to curItem's coordinate system
        QPointF endPoint = begPoint + delta;  //this point is a position of otherItem after 0.5 sec in curItem's coordinate system (of course if current items dont change velocity)
        line->setNewPoints(begPoint, endPoint);

        if(line->intersects(pRect)) //if line(that very long) is intersects pixmapRect we return it
            return line;
        //delete unique_ptr and object Line
    }

    return nullptr;
}

Vector2D FirstShipStrategy::avaidObjectVelocity()
{
    Vector2D resultVelocity;
    for(auto& elem: avaidObjects)
        resultVelocity += elem;
    resultVelocity = V2Extend::Normalize(resultVelocity);

    avaidObjects.erase(avaidObjects.begin(), avaidObjects.end());

    return resultVelocity * AllObjectParam::getSingle()->
                                currentParam(currentItem()->idObject())
            ->acceleration;
}


//calculate in scene coordinate system
Vector2D FirstShipStrategy::attackTargetVelocity(std::shared_ptr<Object> target)
{
    QPointF enemyCenter(target->pixmapItem()->pos());
    QPointF curCenter(currentItem()->pixmapItem()->pos());
    Vector2D vectToEnemy(enemyCenter - curCenter);

    qreal distance = vectToEnemy.x() * vectToEnemy.x()  + vectToEnemy.y() * vectToEnemy.y();
    distance -= normDistanceSquare;

//    if(distance < 25 && distance > 0)
//        return Vector2D(0, 0);

    Vector2D attackVelocity = vectToEnemy;
    if(distance < 0)
        attackVelocity = V2Extend::Negative(attackVelocity);
    else if (distance < 200)
        attackVelocity  = attackVelocity  * ((distance )/200);

    attackVelocity = V2Extend::Normalize(attackVelocity);
    return attackVelocity *  AllObjectParam::getSingle()->
                                currentParam(currentItem()->idObject())
            ->acceleration;;
}

qreal FirstShipStrategy::attackTargetAngle(std::shared_ptr<Object> target)
{
    //calculate angle from curItem to otherItem

    qreal angleRad = calculateAngle(QPointF(0, 0),
                                    currentItem()->pixmapItem()->mapFromScene(
                                        target->pixmapItem()->pos()));

    auto limitRotation = AllObjectParam::getSingle()->currentParam(currentItem()->idObject())->limitRotation;
    if(angleRad > limitRotation)
        angleRad = limitRotation;
    else if (angleRad < -limitRotation)
        angleRad = -limitRotation;


    return angleRad/5;
}



//QVector3D FirstShipStrategy::nextStep()
//{
//    curWeapon->decreaseTicks();
//    if(ifBumb())
//        return bumbTrajectory();
//    //begin calculating prior velocity

//    EdgeVelocity bulletPrior;
//    if(avaidBullet.size() > 0)
//    {
//        for(auto& elem: avaidBullet)
//            bulletPrior = bulletPrior + elem;
//        avaidBullet.erase(avaidBullet.begin(), avaidBullet.end());
//    }
//   bulletPrior = bulletPrior * 3;

//    EdgeVelocity itemPrior;
//    if(avaidItem.size() > 0)
//    {
//        for(auto& elem: avaidItem)
//            itemPrior = itemPrior + elem;
//        avaidItem.erase(avaidItem.begin(), avaidItem.end());
//    }

//    EdgeVelocity res = bulletPrior + itemPrior;
//    if(res.normalize())
//    {
//        res = res.rotate(currentItem()->pixmapItem->rotation());

//    }
////    else
////        res = attakCalculate(MainItem::getMainItem()->getObject());


//    currentItem()->velocity.setX(currentItem()->velocity.x() + res.right * currentItem()->acceleration);
//    currentItem()->velocity.setY(currentItem()->velocity.y() + res.bottom * currentItem()->acceleration);
//    currentItem()->checkVelocity();

//    return GameStrategy::deltaStep();
//}


//void FirstShipStrategy::setDistance(qreal dist)
//{
//    normDistance = dist;
//}

//bool FirstShipStrategy::shouldDoShot() const
//{
//    qreal angleDegree = rotateCalculate(MainItem::getMainItem()->getObject());
//    bool ifWeShoot = false;
//    if(angleDegree < 1)
//    {
////        ifWeShoot = true;
//    }

//    angleDegree /= 5;

//    if(std::abs(angleDegree) < 1)
//        currentItem()->w += currentItem()->rotateAcceleration * angleDegree;
//    else
//    {
//        currentItem()->w += currentItem()->rotateAcceleration * (angleDegree > 0 ? 1 : -1);
//    }

//    return ifWeShoot;
//}






////we have point, we have rect --> we must find point on one of the rect's border
//FirstShipStrategy::Border FirstShipStrategy::findBorderByPoint(const QPointF& pp, const QRectF& rect) const
//{
//     if(pp.x() == rect.left())
//         return Border::Left;

//     if(pp.x() == rect.right())
//         return Border::Right;

//     if(pp.y() == rect.top())
//         return Border::Top;

//     if(pp.y() == rect.bottom())
//         return Border::Bottom;

//     return Border::None;
//}

////find line between moving otherItem and not moving curItem [ we take velocity from item and add it to other velocity]
//// calculate borders of otherItem's pixmapRect and answer the question: If our otherItem is shot current item? and what line is that?
//std::unique_ptr<Line2D> FirstShipStrategy::findLineIntersection(const std::shared_ptr<Object> &otherItem) const
//{
//    //we want to go to the curItem's coorinate system -- in this system our item has got velocity = 0, 0;

//    QRectF pRect(currentItem()->pixmapItem->boundingRect());
//    QPointF velGlobal = QPointF(otherItem->velocity.x() - currentItem()->velocity.x(),
//                                     otherItem->velocity.y() - currentItem()->velocity.y());  //curItem's velocity = 0

//    qreal rad = currentItem()->pixmapItem->rotation() * fromDegreesToRad;
//    QPointF bulletVelocity;
//    bulletVelocity.setX(velGlobal.x() * std::cos(rad) + velGlobal.y() * std::sin(rad));
//    bulletVelocity.setY(velGlobal.y() * std::cos(rad) - velGlobal.x()* std::sin(rad));



//    //create vector with corners in otherItem's coordinate system [for more comfortable find]
//    QVector<QPointF> corners{otherItem->pixmapItem->pixmapRect().center(),
//                                otherItem->pixmapItem->pixmapRect().topLeft() + QPointF(-3, -3),
//                                otherItem->pixmapItem->pixmapRect().topRight() + QPointF(3, -3),
//                                otherItem->pixmapItem->pixmapRect().bottomRight() + QPointF(3, 3),
//                                otherItem->pixmapItem->pixmapRect().bottomLeft() + QPointF(-3, 3)};

//    const QPointF delta = 0.5 * bulletVelocity;

//    for(auto& begPoint: corners)
//    {
//        begPoint = otherItem->pixmapItem->mapToItem(currentItem()->pixmapItem, begPoint); //from otherItem's to curItem's coordinate system
//        QPointF endPoint = begPoint + delta;  //this point is a position of otherItem after 0.5 sec in curItem's coordinate system (of course if current items dont change velocity)
//        std::unique_ptr<Line2D> line(new Line2D(begPoint, endPoint)); //create unique

//        if(line->intersects(pRect)) //if line(that very long) is intersects pixmapRect we return it
//            return line;
//        //delete unique_ptr and object Line
//    }

//    return nullptr;
//}


//FirstShipStrategy::EdgeVelocity FirstShipStrategy::bumbWithObject(const std::shared_ptr<Object> &otherItem) const
//{
//    //all in curItem's local coordinate
//    qreal h = currentItem()->pixmapItem->boundingRect().height();
//    qreal w = currentItem()->pixmapItem->boundingRect().width();
//    QVector<QPointF> corner{QPointF(-w, -h), //leftTop
//                            QPointF(w, -h), //rightTop
//                            QPointF(w, h),//rightBottom
//                            QPointF(-w, h)};//leftBottom



//    //find bullet trajectory that intersection currentItem
//    std::unique_ptr<Line2D> line = findLineIntersection(otherItem);

//    if(line == nullptr) //if don't find
//        return EdgeVelocity();

//    auto points = line->intersected(currentItem()->pixmapItem->boundingRect()); //find points
//    //and point in points(QVector) are sort by this rule (point on LeftBorder < point on TopBorder < RightBord < BottomBorder)


//    EdgeVelocity ev; //create results of current calculating

//    if(points.size() == 2) //it will be always
//    {
//        QVector<Border> interBorder; //detect what border are intersect
//        int summ = 0;

//        for(int i = 0; i < 2; i ++)
//        {
//            auto bord = findBorderByPoint(points[i], currentItem()->pixmapItem->boundingRect());
//            if(bord == Border::None)
//                continue;
//            interBorder.push_back(bord);
//            summ += static_cast<int>(bord);
//        }

//        if(summ % 2 != 0)  //this means that bullet contact with two edges that near..
//            //[ for example: LEFT = 0 edge and TOP = 1  edge = (1 % 2 != 0) --- two edges that near [ or RIGHT = 2 and TOP  = 1or RUGHT and BOTTOM ....]
//        {
//            qreal splitFirst = 0.5, splitSecond = 0.5;
//            int i = static_cast<int>(interBorder[0]);

//            if(interBorder[0] == Border::Left || interBorder[0] == Border::Right)
//            {
//                splitFirst -= points[0].y()/corner[i].y();
//                splitSecond -= points[1].x()/corner[i].x();
//            }
//            else
//            {
//                splitFirst -= points[0].x()/corner[i].x();
//                splitSecond -= points[1].y()/corner[i].y();
//            }


//            ev.addValue(interBorder[0], -splitFirst);

//            ev.addValue(interBorder[1], -splitSecond);

//        }
//        else  //this if means that bullet contact with two edges that in front of each other...[ for example: TOP and BOTTOM or LEFT and RIGHT
//        {
//            qreal splitFirst = 0.5, splitSecond = 0.5;
//            int i = static_cast<int>(interBorder[0]);

//            //splitFirst can be negative??? Look horizontal trajectory
//            if(interBorder[0] == Border::Left)
//            {
//                splitFirst -= points[0].y()/corner[i].y();
//                splitSecond -= points[1].y()/corner[i + 2].y();
//            }
//            else if (interBorder[0] == Border::Top)
//            {
//                splitFirst -= points[0].x()/corner[i].x();
//                splitSecond -= points[1].x()/corner[i + 2].x();
//            }
//            else
//            {
//                qDebug()<<"It's strange!!!!";
//                system("pause");
//            }


//            int vectOfDviz = 0;


//            QPointF d1 = otherItem->pixmapItem->mapToItem(currentItem()->pixmapItem, QPointF(0, 0))- points[0];
//            QPointF d2 = otherItem->pixmapItem->mapToItem(currentItem()->pixmapItem, QPointF(0, 0)) - points[1];

//            if((d1.x() * d1.x() + d1.y() * d1.y()) < (d2.x() * d2.x() + d2.y() * d2.y()))
//            {
//                ev.addValue(interBorder[1], 1);
//                vectOfDviz = (static_cast<int>(interBorder[0]) + 1) % 4;

//            }
//            else
//            {

//                ev.addValue(interBorder[0], 1);
//                vectOfDviz = (static_cast<int>(interBorder[1]) + 1) % 4;

//            }

//            if(splitFirst < 0.25)
//                splitFirst = 0.25;

//            if(splitSecond < 0.25)
//                splitSecond = 0.25;


//            ev.addValue(static_cast<Border>(vectOfDviz), (splitFirst > splitSecond ? splitFirst: splitSecond));
//        }
//    }

//    return ev;
//}

//FirstShipStrategy::EdgeVelocity FirstShipStrategy::attakCalculate(const std::shared_ptr<Object> &otherItem) const
//{
//    QPointF enemyCenter(otherItem->pixmapItem->pos());
//    QPointF curCenter(currentItem()->pixmapItem->pos());
//    QVector2D vectToEnemy(enemyCenter - curCenter);
//    EdgeVelocity ev(vectToEnemy.x(), vectToEnemy.y());
//    ev.normalize();

//    qreal distance = static_cast<qreal>(vectToEnemy.length());
//    distance -= normDistance;

//    if(distance < 0)
//    {
//        ev.right *= -1;
//        ev.bottom *= -1;
//    }
//    else if(distance < 100)
//    {
//        ev = ev * (distance/100);
//    }

//    return ev;


//}

//qreal FirstShipStrategy::rotateCalculate(const std::shared_ptr<Object> &otherItem) const
//{
//    //calculate angle from curItem to otherItem

//    qreal angleDegree = fromRadToDegrees * calculateAngle(QPointF(0, 0),
//                                                          currentItem()->pixmapItem->mapFromScene(
//                                                              otherItem->pixmapItem->pos()));
//    normalizeAngle(angleDegree);
//    if(angleDegree > currentItem()->limitRotation)
//        angleDegree = currentItem()->limitRotation;
//    else if (angleDegree < -currentItem()->limitRotation)
//        angleDegree = -currentItem()->limitRotation;
//    return angleDegree;
//}


//void FirstShipStrategy::EdgeVelocity::addValue(const FirstShipStrategy::Border &b, const qreal &value)
//{
//    switch(b)
//    {
//    case Border::Left:
//        right -= value;
//        break;
//    case Border::Top:
//        bottom -= value;
//        break;
//    case Border::Right:
//        right += value;
//        break;
//    case Border::Bottom:
//        bottom += value;
//        break;
//    default:
//        break;
//    }

//    return;
//}



//bool FirstShipStrategy::EdgeVelocity::normalize()
//{
//    qreal norm = std::sqrt(right * right + bottom * bottom);
//    if(norm == 0)
//        return false;
//    bottom = bottom / norm;
//    right = right / norm;
//    return true;
//}

//FirstShipStrategy::EdgeVelocity FirstShipStrategy::EdgeVelocity::rotate(const qreal &degree) const
//{
//    EdgeVelocity res;
//    qreal rad = degree * fromDegreesToRad;
//    res.right = right * std::cos(rad) + bottom * std::sin(rad);
//    res.bottom = bottom * std::cos(rad) - right * std::sin(rad);
//    return res;
//}

//const FirstShipStrategy::EdgeVelocity FirstShipStrategy::EdgeVelocity::operator+(const FirstShipStrategy::EdgeVelocity &other)
//{
//    EdgeVelocity res;
//    res.right = right + other.right;
//    res.bottom = bottom + other.bottom;
//    return res;
//}

//FirstShipStrategy::EdgeVelocity &FirstShipStrategy::EdgeVelocity::operator*=(const FirstShipStrategy::EdgeVelocity &ed)
//{
//    right *= ed.right;
//    bottom *= ed.bottom;
//    return *this;
//}

//FirstShipStrategy::EdgeVelocity FirstShipStrategy::EdgeVelocity::operator*(FirstShipStrategy::EdgeVelocity first)
//{
//    first *= *this;
//    return first;
//}


//template<class T>
//FirstShipStrategy::EdgeVelocity &FirstShipStrategy::EdgeVelocity::operator*=(T i)
//{
//    right *= i;
//    bottom *= i;
//    return *this;
//}

//template<class T>
//FirstShipStrategy::EdgeVelocity FirstShipStrategy::EdgeVelocity::operator*(T i)
//{
//    EdgeVelocity ed(*this);
//    ed *= i;
//    return ed;
//}


//const std::shared_ptr<ChangeObjectData> FirstShipStrategy::nextStep()
//{
//}

//void FirstShipStrategy::avaidBumbWithObject(const std::shared_ptr<Object> &)
//{
//}

