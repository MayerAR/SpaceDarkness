#include "gameWidget/objects/object.h"
#include "gameWidget/strategy/InterfaceStrategy.h"
#include "gameWidget/objects/differentObjects/pixmapParametres/pixmapitem.h"
#include "gameWidget/objects/differentObjects/AllObjectParam/allobjectparam.h"
#include "gameWidget/objects/changeobjectdata.h"
#include <QGraphicsScene>
#include <QDebug>

Object::Object(const QString &pixmapPath, const QSize &size, objectID curID_,strategyID strID_):
    curID(curID_),
    velocity(Vector2D(0, 0)),
    zVelocity(0),
    pItem(nullptr),
    inv_momentI(1),
    curStrategy(nullptr)
{
    //error!!! virtual methode
    setPixmapItem(pixmapPath, size); //set new pixmap
    changeStrategy(strID_); //set new strategy
}

Object::~Object()
{
    if(pItem->scene() != nullptr)
    {
        pItem->scene()->removeItem(pItem.get());
    }
}

void Object::addToScene(QGraphicsScene *scene)
{
    if(pItem.get() == nullptr)
        return;
    scene->addItem(pItem.get());
}

const Vector2D &Object::curVelocity() const
{
    return velocity;
}

qreal Object::rotationVelocity() const
{
    return zVelocity;
}

qreal Object::rotationAngleRad() const
{
    return pItem->getAngleRad();
}

qreal Object::invImpulse() const
{
    return inv_momentI;
}

const QVector<Edge> &Object::localEdges() const
{
    return pItem->localEdges();
}

const QVector<Edge> &Object::sceneEdges() const
{
    return pItem->sceneEdges();
}

const std::shared_ptr<QGraphicsPixmapItem> Object::pixmapItem() const
{
    return pItem;
}

std::shared_ptr<AbstractStrategy> Object::currentStrategy()
{
    return curStrategy;
}

objectID Object::idObject() const
{
    return curID;
}

void Object::addChildItem(QGraphicsItem* childItem)
{
    childItem->setParentItem(pItem.get()); //set child to current object
}

QRectF Object::localPixmapRect() const
{
    return pItem->pixmapRect();
}

void Object::setBoundingRect(const QSize &size)
{
    pItem->setBoundingRect(size);

//    QGraphicsRectItem* rect = new QGraphicsRectItem(pItem.get());
//    rect->setRect(pItem->boundingRect());
//    rect->setPen(QPen(QBrush(QColor(255, 255 , 255)) , 2));
}

void Object::setPixmap(const QPixmap& p)
{
    auto pRect = pItem->pixmapRect();
    QSize size(pRect.width(), pRect.height());

    p.scaled(size, Qt::IgnoreAspectRatio); //set parametres of new pixmap
    pItem->setPixmap(p);
    pItem->setOffset(-(size.width()>>1),
                     -(size.height()>>1));
}

void Object::setPosition(const QPointF &pos)
{
    pItem->setPos(pos);
}


void Object::changeStrategy(strategyID newID)
{
    auto newStrategy = curStrategy;
    newStrategy.reset(AbstractStrategy::createStrategy(newID, this));
    swap(curStrategy, newStrategy);
}

qreal Object::acceleration() const
{
    return AllObjectParam::getSingle()->currentParam(idObject())->acceleration;
}

qreal Object::limitVelocity() const
{
    return AllObjectParam::getSingle()->currentParam(idObject())->limitVelocity;

}

int Object::limitRotation() const
{
    return AllObjectParam::getSingle()->currentParam(idObject())->limitRotation;
}

bool Object::isStatic() const
{
    return AllObjectParam::getSingle()->currentParam(idObject())->isStatic;
}

qreal Object::elasticity() const
{
    return AllObjectParam::getSingle()->currentParam(idObject())->elasticity;
}

qreal Object::mass() const
{
    return AllObjectParam::getSingle()->currentParam(idObject())->mass;
}

qreal Object::inv_mass() const
{
    return AllObjectParam::getSingle()->currentParam(idObject())->inv_mass;
}


void Object::update(const std::shared_ptr<ChangeObjectData> upData)
{
    //in this virtual function we change all parametres of current Object
    doUpdate(upData);

    checkVelocity(); //we must check velocity after every changes!
    pItem->setNeedUpdate(); //we must set PixmapItem::ifUpdate in position false ---> we need to update edges of current pixmap rect
    curStrategy->update();
}

void Object::doUpdate(const std::shared_ptr<ChangeObjectData> upData)
{
    //basic action to update
    pItem->moveBy(upData->deltaPosition.x(), upData->deltaPosition.y());
    velocity += upData->deltaVelocity;

    zVelocity += upData->deltaZVelocity;

    pItem->setDeltaRotation(upData->angleRad);
}

void Object::checkVelocity()
{
    auto limitVelocity = AllObjectParam::getSingle()->currentParam(idObject())->limitVelocity;
    auto limitRotation = AllObjectParam::getSingle()->currentParam(idObject())->limitRotation;

    if(velocity.x() > limitVelocity)
        velocity.setX(limitVelocity);
    else if(velocity.x() < -limitVelocity)
        velocity.setX(-limitVelocity);

    if(velocity.y() > limitVelocity)
        velocity.setY(limitVelocity);
    else if(velocity.y() < -limitVelocity)
        velocity.setY(-limitVelocity);

    if(zVelocity > limitRotation)
        zVelocity = limitRotation;
    else if(zVelocity < -limitRotation)
        zVelocity = -limitRotation;
}

void Object::setPixmapItem(const QString &pixmapPath, const QSize &size)
{

    QPixmap p(size);


    if(pixmapPath == "")
        p.fill(Qt::white);
    else if(p.load(pixmapPath))
        p = p.scaled(size, Qt::IgnoreAspectRatio);
    else
        qDebug()<<"wrong path!";


    switch (curID)
    {
    case objectID::stdBullet:
        pItem = std::make_shared<bulletPixmap>(p, size);
        break;
    default:
        pItem = std::make_shared<PixmapItem>(p, size);
    }

    calculateMoment();
}

void Object::calculateMoment()
{
    qreal a = pItem->pixmapRect().width();
    qreal b = pItem->pixmapRect().height();
    qreal mass = AllObjectParam::getSingle()->currentParam(idObject())->mass;

    inv_momentI = 12 / ( mass * (a*a + b*b) ); //calculate inv moment of rectangle. momentI = mass(a*a + b*b)/12
}
