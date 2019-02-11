#ifndef EDGE_H
#define EDGE_H

#include "gameWidget/constants.h"

class Edge //need for describe border of pixmap item
{
public:
    Edge();
    Edge(const Vector2D& n_, const qreal& d_);
    Edge(const Edge& newEdge);

    Edge& operator=(const Edge& other);

    const Vector2D& normal() const;
    qreal distance() const;
    void setNormal(const Vector2D& norm);
    void setDistance(qreal dist);

private:
    Vector2D n; //normal
    qreal d; //distanse to Edge[from point (0, 0)]
};

#endif // EDGE_H
