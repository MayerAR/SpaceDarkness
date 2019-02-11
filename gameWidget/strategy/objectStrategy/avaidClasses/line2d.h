#ifndef LINE2D_H
#define LINE2D_H

#include <QPointF>
#include "gameWidget/constants.h"

//this class needs for calculate intersects of line and rect
            //[ for calculate bullet's trajectory and intersects with different gameItem
class Line2D
{
public:
    Line2D();
    Line2D(const QPointF& first, const QPointF& second);
    Line2D(const Line2D& other);
    Line2D& operator=(const Line2D& other);

    void setNewPoints(const QPointF& p1, const QPointF& p2);
    //p - gameItem's pixmap coordinate[ in gameItem's coordinate system]
    QVector<QPointF> intersected(const QRectF& p) const;
    bool intersects(const QRectF& r) const;

private:
    bool ifTopLine(const QPointF& p) const;

    QPointF firstPoint;
    Vector2D vect;
    Vector2D perpVect; //perpVect is init by vect
};


#endif // LINE2D_H
