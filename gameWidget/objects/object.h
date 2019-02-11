#ifndef FACTORYCLASS_H
#define FACTORYCLASS_H

#include <memory>
//#include <QPointF>
//#include <QRectF>
#include <QVector>
#include "gameWidget/constants.h"

struct ChangeObjectData;

class Edge;
class PixmapItem;
class AbstractStrategy;
class QRectF;
class QGraphicsPixmapItem;
class QRect;
class QPointF;
class QPixmap;
class QGraphicsScene;
class QGraphicsItem;



//all of game characters
class Object
{
public:
    Object(const QString& pixmapPath, const QSize &size, objectID curID_, strategyID strID_);
    virtual ~Object();

    //take information from class
    void addToScene(QGraphicsScene* scene);
    const Vector2D& curVelocity() const;
    qreal rotationVelocity() const;
    qreal rotationAngleRad() const;
    qreal invImpulse() const;
    const QVector<Edge>&  localEdges() const;
    const QVector<Edge>&  sceneEdges() const;
    const std::shared_ptr<QGraphicsPixmapItem> pixmapItem() const; //all QGraphicsPixmapItem data from returned shared_ptr
    std::shared_ptr<AbstractStrategy> currentStrategy(); 

    objectID idObject() const;
    virtual bool ifAlive() const = 0;

    void addChildItem(QGraphicsItem* childItem);

    //pixmap rect in local coordinate
    QRectF localPixmapRect() const;
    //set information
    void setBoundingRect(const QSize& size);
    void setPixmap(const QPixmap& p);
    void setPosition(const QPointF& pos);
    void changeStrategy(strategyID newID);


    /////
    //return const ObjectParam from AllObjectParam
    qreal acceleration() const;
    qreal limitVelocity() const;
    int limitRotation() const;
    bool isStatic() const;
    qreal elasticity() const;
    qreal mass() const;
    qreal inv_mass() const;


    //static function, that call virtual doUpdate(...)
    void update(const std::shared_ptr<ChangeObjectData> upData); //checkVelocity must be here!!!!
protected:
    virtual void doUpdate(const std::shared_ptr<ChangeObjectData> upData);

private:
    void checkVelocity();
    void setPixmapItem(const QString &pixmapPath, const QSize &size);
    void calculateMoment();

    objectID curID;
    Vector2D velocity;
    qreal zVelocity;
    std::shared_ptr<PixmapItem> pItem;
    qreal inv_momentI;//  1 / moment of Inerеtia
    std::shared_ptr<AbstractStrategy> curStrategy;//shared_ptr
};


#endif // FACTORYCLASS_H
