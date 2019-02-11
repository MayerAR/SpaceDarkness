#include "line2d.h"
#include <QVector>
#include <QRectF>

Line2D::Line2D():
    Line2D::Line2D(QPointF(0, 0), QPointF(0, 0))
{
}

//Line2D class...
//create line like a x = x1 + vx * L;   y = y1 + vy * L
Line2D::Line2D(const QPointF &first, const QPointF &second):
    firstPoint(first), //(x1, y1)
    vect(second - first) //(vx, vy)

{
    vect = V2Extend::Normalize(vect);
    perpVect = V2Extend::Perp(vect);
}

Line2D::Line2D(const Line2D &other):
    firstPoint(other.firstPoint),
    vect(other.vect),
    perpVect(other.perpVect)
{
}

Line2D& Line2D::operator=(const Line2D &other)
{
    if(this == &other)
        return *this;

    firstPoint = other.firstPoint;
    vect = other.vect;
    perpVect = other.perpVect;
    return *this;
}

void Line2D::setNewPoints(const QPointF &p1, const QPointF &p2)
{
    firstPoint = p1;
    vect = V2Extend::Normalize(Vector2D(p2 - p1));
    perpVect = V2Extend::Perp(vect);
}



QVector<QPointF> Line2D::intersected(const QRectF &p) const
{
    QVector<QPointF> interPoint;
    const qreal splitV = vect.y()/vect.x();

    qreal y = firstPoint.y() + splitV * (p.left() - firstPoint.x()); //we find y - value with x = p.left()
    if (y >= p.topLeft().y() && y < p.bottomLeft().y())
        interPoint.push_back(QPointF(p.left(), y));

    qreal x = firstPoint.x() + (p.top() - firstPoint.y()) / splitV; //we find x - value with y = p.top()
    if(x <= p.topRight().x() && x > p.topLeft().x())
        interPoint.push_back(QPointF(x, p.top()));


    y = firstPoint.y() + splitV * (p.right() - firstPoint.x());
    if (y > p.topRight().y() && y <= p.bottomRight().y())
        interPoint.push_back(QPointF(p.right(), y));

    x = firstPoint.x() + (p.bottom() - firstPoint.y()) / splitV;
    if(x < p.bottomRight().x() && x >= p.bottomLeft().x())
        interPoint.push_back(QPointF(x, p.bottom()));

    return interPoint;
}

//if all of the vertex of QRectF r is on the one side relatively current line --> this line dont intersect current rect
//if one of vertex of RectF r is on the other side --> this line intersect current rect --> return true;
bool Line2D::intersects(const QRectF &r) const
{
    bool topLeft = ifTopLine(r.topLeft());
    bool topRight = ifTopLine(r.topRight());
    if(topLeft != topRight) //if on the other side
        return true; //our line and rect are intersect

    bool bottomLeft = ifTopLine(r.bottomLeft());
    if(topLeft != bottomLeft)
        return true;

    bool bottomRight = ifTopLine(r.bottomRight());
    if(topLeft != bottomRight)
        return true;

    return false;
}

//we must check if point p is on the top side of current line
    //top is in the positive direction of perpVect...
bool Line2D::ifTopLine(const QPointF &p) const
{
    Vector2D toPoint(p - firstPoint); //create vector: from point on line firstPoint to point p

    return V2Extend::Dot(perpVect, toPoint) > 0; // A > B
                                                    //A : "proekzia" on the vector to Point
}
