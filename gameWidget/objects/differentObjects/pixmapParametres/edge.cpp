#include "edge.h"


//class Edge
Edge::Edge():
    n(Vector2D(0, 0)),
    d(0)
{}

Edge::Edge(const Vector2D &n_, const qreal &d_):
    n(n_),
    d(d_)
{}

Edge::Edge(const Edge &newEdge):
    n(newEdge.normal()),
    d(newEdge.distance())
{}

Edge &Edge::operator=(const Edge &other)
{
    if(this != &other)
    {
        this->setNormal(other.normal());
        this->setDistance(other.distance());
    }
    return *this;
}

const Vector2D &Edge::normal() const
{
    return n;
}

qreal Edge::distance() const
{
    return d;
}

void Edge::setNormal(const Vector2D &norm)
{
    if(norm != Vector2D(0, 0))
        n = norm;
}

void Edge::setDistance(qreal dist)
{
    d = dist;
}
