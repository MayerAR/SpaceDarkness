#ifndef CONTACT_H
#define CONTACT_H

#include <QVector>
#include <memory>
#include "gameWidget/constants.h"

class Edge;
class Object;
class GameObject;
class QPolygonF;
class BulletObject;

//class
class Contact
{
public:
    //returt true if they really bumb
    //GameObject and GameObject
    static bool bumb(std::shared_ptr<Object> p1, std::shared_ptr<Object> p2);
    //GameObject adnd BulletObject
    static bool bumb(std::shared_ptr<GameObject> i, std::shared_ptr<BulletObject> b);
    static bool bumb(std::shared_ptr<BulletObject> b, std::shared_ptr<GameObject> i);
private:
    //find min distance from point to other item's edge
    static bool FindCollision(std::shared_ptr<Object> p1, std::shared_ptr<Object> p2);
    static QPolygonF collidingVertex(const std::shared_ptr<Object> p1, const std::shared_ptr<Object> p2);
    static bool PointIn(const std::shared_ptr<Object> gg, Vector2D p);
    static int FindEdgeMinDist(const QPolygonF& pp, const QVector<Edge>& ed, int num, qreal& min_);
    static qreal EdgeDist(const QPolygonF& pp, Vector2D n, qreal d);

    static void VertsProc(std::shared_ptr<Object>  p1, std::shared_ptr<Object> p2, const QPolygonF &pointsOfContact, Vector2D n, qreal d);

    void addPoint(const Vector2D& newPoint);
    void Solve(std::shared_ptr<Object> c1, std::shared_ptr<Object> c2);

    static qreal c_depth;
    Vector2D p; // intersection point
    Vector2D n; //normal form body 1 to body 2
    Vector2D r1; //vector to intersection point[p] from body 1
    Vector2D r2; //vector to intersection point[p] from body 2
    qreal depth; //penetration depth
};

#endif // CONTACT_H
