#include "pixmapitem.h"
#include "gameWidget/constants.h"
#include "gameWidget/objects/differentObjects/pixmapParametres/edge.h"
#include <cmath>


//class PixmapItem
PixmapItem::PixmapItem(const QPixmap& pixmap, const QSize& size, QGraphicsItem *parent):
     QGraphicsPixmapItem(pixmap, parent),
     bRect(-pixmap.width()/2, -pixmap.height()/2, pixmap.width(), pixmap.height()),
     angleRad(0),
     isUpdate(false)
{

    this->setOffset(-(size.width()>>1),
                     -(size.height()>>1));
    loadEdge();
}

PixmapItem::~PixmapItem()
{

}

QRectF PixmapItem::boundingRect() const
{
    return bRect;
}

QPainterPath PixmapItem::shape() const
{
    QPainterPath p;
    p.addRect(bRect);
    return p;
}
//pixmap position in local coordinate
QRectF PixmapItem::pixmapRect() const
{
    QRectF resRect(this->offset(), this->pixmap().size());
    return resRect;
}

void PixmapItem::setBoundingRect(const QSize &size)
{
    bRect = QRectF(-size.width()/2, -size.height()/2, size.width(), size.height());
}

void PixmapItem::destructionOfPixmap(const QPointF &/*localPoint*/, qreal /*damage*/)
{
    return;
}

//angle in radian
void PixmapItem::setDeltaRotation(qreal radian)
{
    angleRad += radian;
    angleRad = normalizeAngle(angleRad);
    QGraphicsPixmapItem::setRotation(angleRad * fromRadToDegrees); //this eat only degrees, but every angle in our programm is a radian

    setNeedUpdate();
}

const QVector<Edge> &PixmapItem::localEdges()
{
    updateEdges(); //if we need this data we must update this
    return ed_base;
}

const QVector<Edge> &PixmapItem::sceneEdges()
{
    updateEdges(); //if we need this data we must update this
    return ed;
}

qreal PixmapItem::getAngleRad() const
{
    return angleRad;
}

//update current edge
void PixmapItem::updateEdges()
{
    if(isUpdate) //if allready update
        return;

    Vector2D rot(std::cos(angleRad), //create rot for more easy calclate rotate
                    std::sin(angleRad));

    for(int i = 0, size = ed.size(); i < size; i++)
    {
        ed[i].setNormal(V2Extend::Rotate(ed_base[i].normal(), rot));
        ed[i].setDistance(ed_base[i].distance() + V2Extend::Dot(
                                                            this->pos(),
                                                            ed[i].normal()));
    }

    isUpdate = true;
}

void PixmapItem::loadEdge()
{
    QPolygonF localPolygon(pixmapRect());
    auto allVertex = localPolygon.toStdVector();
    allVertex.pop_back(); //because polygon's vertex are [A, B, C, D, A] -- A is twice
    //allVertex save [A, B, C, D] vertex
    QPointF previousV = allVertex.back(); // save D Vertex

    for(size_t i = 0, size = allVertex.size();
        i < size;
        i++)
    {
        Vector2D perpEdge = V2Extend::Perp( //create perp vector to current edge(for calculate normal to edge)
                    Vector2D(previousV - allVertex[i])   //vector(i): vector(0) -- DA, vector(1) -- (AB), ... vector(3) -- (CD)
                    );

        ed_base.push_back(Edge());
        ed_base.back().setNormal(V2Extend::Normalize(perpEdge));//vector n [ in base coordinate]
        ed_base.back().setDistance(V2Extend::Dot(  //distance to the edge from point (0, 0) [in base coordinate]
                                                   ed_base.back().normal(),
                                                   Vector2D(allVertex[i])
                                                   ));

        ed.push_back(Edge());

        previousV = allVertex[i]; //next vertex from D to A
    }

}


void PixmapItem::setNeedUpdate()
{
    isUpdate = false;
}



//bulletPixmap
bulletPixmap::bulletPixmap(const QPixmap &pixmap, const QSize &size, QGraphicsItem *parent):
    PixmapItem(pixmap, size, parent)
{}

bulletPixmap::~bulletPixmap()
{

}

void bulletPixmap::setBoundingRect(const QSize &size)
{
    bRect = QRectF(-pixmap().rect().width()/2, -size.height()/2, size.width(), size.height()); //we must to shift a little
}


