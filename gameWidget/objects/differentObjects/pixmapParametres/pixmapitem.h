#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include "gameWidget/constants.h"

class Edge;

//this is class that includes parametres of a square body
//it must save and update pixmap's parametres
class PixmapItem: public QGraphicsPixmapItem
{
public:
    PixmapItem(const QPixmap &pixmap, const QSize& size, QGraphicsItem* parent = nullptr);
    ~PixmapItem() override;

    QPainterPath shape() const override;
    QRectF pixmapRect() const;
    QRectF boundingRect() const override;

    virtual void setBoundingRect(const QSize& size);


    //it needs for change pixmapItem, when they are destraction
    virtual void destructionOfPixmap(const QPointF& localPoint, qreal damage);

    void setDeltaRotation(qreal radian);

    void setNeedUpdate(); //set isUpdate in false;

    //methodes that return data
    const QVector<Edge>& localEdges();
    const QVector<Edge>& sceneEdges();
    qreal getAngleRad() const;
protected:
    //it's need for update different parametres of current pixmap item ( like local and scene edge)
    void updateEdges();

    void loadEdge();

    QRectF bRect; //bounding rect of current pixmapItem
    //this bounding Rect is need for find collisions with different object.
    //this is not pixmapRect!!!
private:

    qreal angleRad;
    bool isUpdate; 
    QVector<Edge> ed; // scene data of edges [n, d]
    QVector<Edge> ed_base; //local data of edges
};

//this class need for change boundingRect
class bulletPixmap: public PixmapItem
{
public:
    bulletPixmap(const QPixmap& pixmap, const QSize& size, QGraphicsItem* parent = nullptr);
    ~bulletPixmap() override;
    void setBoundingRect(const QSize& size) override;
};


#endif // PIXMAPITEM_H
