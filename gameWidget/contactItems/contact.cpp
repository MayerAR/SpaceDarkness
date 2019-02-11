#include "contact.h"
#include <QDebug>
#include "gameWidget/objects/allobjects.h"
#include "gameWidget/objects/differentObjects/pixmapParametres/edge.h"
#include "gameWidget/strategy/InterfaceStrategy.h"
#include "gameWidget/animationItem/animationItem.h"
#include "gameWidget/loadStage/loadStage.h"
#include <QGraphicsPixmapItem>
#include <QPolygonF>
#include <cmath>


//Contact that handles collisions
qreal Contact::c_depth = 3; // 2.5

bool Contact::bumb(std::shared_ptr<Object> p1, std::shared_ptr<Object> p2)
{
    return Contact::FindCollision(p1, p2);
}

bool Contact::bumb(std::shared_ptr<GameObject> i, std::shared_ptr<BulletObject> b)
{
    QPolygonF itemPolygon(i->localPixmapRect());
    auto bulletPolygon = b->pixmapItem()->mapToItem(i->pixmapItem().get(), b->localPixmapRect());

    if(bulletPolygon.intersects(itemPolygon)) //bullet damaged item
    {
        if(i->changeHealth(-b->currentDamage())) //если объект еще существует
        {
            auto pointsOfIntersect = bulletPolygon.intersected(itemPolygon).toStdVector();
            pointsOfIntersect.pop_back();
            QPointF resPoint(0, 0);

            for(auto elem: pointsOfIntersect)
                resPoint += elem;

            resPoint /= pointsOfIntersect.size();
            Vector2D impulse( b->currentDamage() * std::cos(b->rotationAngleRad()),//5*
                             b->currentDamage() * std::sin(b->rotationAngleRad()));

            i->currentStrategy()->applyImpulse(impulse, Vector2D(resPoint));

            EndingAnimation* animation = createObject::getSingle()->createAnimationExplosionBullet(b->idBullet(), i->pixmapItem().get());
            animation->setPos(resPoint);
            animation->start();
        }
        return true;
    }
    return false;
}

bool Contact::bumb(std::shared_ptr<BulletObject> b, std::shared_ptr<GameObject> i)
{
    return Contact::bumb(i, b);
}



bool Contact::FindCollision(std::shared_ptr<Object> p1, std::shared_ptr<Object> p2)
{
    qreal min1 = 0.0;
    qreal min2 = 0.0;
    int min1Index = -1, min2Index = -1;

    auto CollidingPolygon2 = Contact::collidingVertex(p2, p1);
    auto CollidingPolygon1 = Contact::collidingVertex(p1, p2);

    QVector<Edge> sceneEdges1;
    QVector<Edge> sceneEdges2;

    if(CollidingPolygon2.size() != 0)
    {
        sceneEdges1 = p1->sceneEdges();
        min1Index = Contact::FindEdgeMinDist(CollidingPolygon2, sceneEdges1, sceneEdges1.size(), min1); //p2.mapToScene(p2.shape()).toFillPolygon()
    }

    if(CollidingPolygon1.size() != 0)
    {
        sceneEdges2  = p2->sceneEdges();
        min2Index = Contact::FindEdgeMinDist(CollidingPolygon1, sceneEdges2, sceneEdges2.size(), min2); //p1.mapToScene(p1.shape()).toFillPolygon()
    }

    for(auto& elem: CollidingPolygon2)
        CollidingPolygon1.push_back(elem);

    if(min1Index >= 0)
        if(min2Index >= 0)
            if(min1 > min2)
                Contact::VertsProc(p1, p2, CollidingPolygon1, sceneEdges1[min1Index].normal(), min1);
            else
                Contact::VertsProc(p1, p2, CollidingPolygon1, V2Extend::Negative(sceneEdges2[min2Index].normal()), min2);
        else
            Contact::VertsProc(p1, p2, CollidingPolygon1, sceneEdges1[min1Index].normal(), min1);
    else if (min2Index >= 0)
        Contact::VertsProc(p1, p2, CollidingPolygon1, V2Extend::Negative(sceneEdges2[min2Index].normal()), min2);
    else
        return false;

    return true;
}

QPolygonF Contact::collidingVertex(const std::shared_ptr<Object> p1, const std::shared_ptr<Object> p2)
{
    QPolygonF localPolygon(p1->localPixmapRect());

    auto vertex = p1->pixmapItem()->mapToParent(localPolygon).toStdVector(); //scene coordinates of cur polygon
    vertex.pop_back();

    QPolygonF res;
    for(size_t i = 0, size = static_cast<size_t>(vertex.size())
        ; i < size
        ; i++)

        if(Contact::PointIn(p2, Vector2D(vertex[i])))
            res.push_back(vertex[i]);

    return res;
}

bool Contact::PointIn(const std::shared_ptr<Object> gg, Vector2D p)
{
    qreal dist;
    auto sceneEdges = gg->sceneEdges();
    for(int i = 0, size =  sceneEdges.size(); i < size; i++)
    {
        dist = V2Extend::Dot(sceneEdges[i].normal(), p) - sceneEdges[i].distance();
        if(dist > 0.0)
            return false;
    }

    return true;
}

int Contact::FindEdgeMinDist(const QPolygonF &pp, const QVector<Edge> &ed, int num, qreal &min_)
{
    if(ed.size() == 0)
    {
        qDebug()<<"Error! graphicsItem::FindEdgeMinDist!!";
        exit(4);
    }


    int minIndex = 0;
    min_ =  Contact::EdgeDist(pp, ed[0].normal(), ed[0].distance());

    qreal dist;
    for(int i = 1; i < num; i++)
    {
        dist = Contact::EdgeDist(pp, ed[i].normal(), ed[i].distance());

        if(dist > 0.0)
            continue;
        else if( dist > min_ || min_ > 0.0) //because dist is a negative number
        {
            min_ = dist;  //we find minimum by module number
            minIndex = i;
        }
    }

    if(min_ > 0.0)
        return -1;

    return minIndex;
}

qreal Contact::EdgeDist(const QPolygonF &pp, Vector2D n, qreal d)
{
    if(pp.count() == 0)
    {
        qDebug()<<"Error! graphicsItem::EdgeDist!";
        exit(3);
    }

    auto vertex = pp.toStdVector();

    qreal min_ = V2Extend::Dot(n, vertex[0]);

    for(size_t i = 1, size = vertex.size(); i < size; i++)
        min_ = std::min( min_, V2Extend::Dot(n, vertex[i]) );

    return min_ - d;
}


void Contact::VertsProc(std::shared_ptr<Object> p1,std::shared_ptr<Object>  p2, const QPolygonF &pointsOfContact, Vector2D n, qreal d)
{
    if(pointsOfContact.size() == 0)
        return;

    qreal k; //body push factor
    d = std::abs(d);
    Contact contact;

    contact.p = Vector2D(pointsOfContact[0]);

    for(int i = 1, size = pointsOfContact.size(); i < size; i++)
        contact.addPoint(Vector2D(pointsOfContact[i]));

    //    p1.pixmapItem->scene()->addRect(contact.p.x() - 2, contact.p.y() - 2, 4, 5, QPen(QColor(255, 0, 0)));

    if(contact.p != Vector2D(-1, -1)) //if we  have contact
    {
        contact.n = n;
        contact.depth = d * Contact::c_depth;
        contact.r1 = contact.p - Vector2D(p1->pixmapItem()->pos());
        contact.r2 = contact.p - Vector2D(p2->pixmapItem()->pos());

        if(p1->isStatic())
            k = 0.0;
        else if (p2->isStatic())
            k = 1.0;
        else
        {
            qreal summ = p1->mass() + p2->mass();
            k = p2->mass()/summ;
        }

        auto distOfDepth = contact.n * contact.depth; //dist that we must to move by p1 and p2
        Vector2D moveP1 = k * distOfDepth;
        p1->setPosition(p1->pixmapItem()->pos() - moveP1);

        Vector2D moveP2 = distOfDepth - moveP1; // or (1 - k) * distOfDepth
        p2->setPosition(p2->pixmapItem()->pos() + moveP2);


        contact.Solve(p1, p2);
    }
}

void Contact::addPoint(const Vector2D &newPoint)
{
    p += newPoint;
    p /= 2;
    return;
}


void Contact::Solve(std::shared_ptr<Object> c1,std::shared_ptr<Object>  c2)
{
    Vector2D rv = c1->curVelocity() - c2->curVelocity();
    qreal velAlongNormal = V2Extend::Dot(rv, n);

    if(velAlongNormal < 0)
        return;

    qreal curE = std::max(c1->elasticity(), c2->elasticity());

    qreal znam = c1->inv_mass() + c2->inv_mass() +
                        std::pow(V2Extend::PerpDot(r1, n), 2) * c1->invImpulse() +
                        std::pow(V2Extend::PerpDot(r2, n), 2) * c2->invImpulse();

    qreal j = -(1+curE) * velAlongNormal;
    j = j / V2Extend::Dot(n, V2Extend::Dot(n, znam));

    Vector2D impulse = j * n;// + j * t;

    c1->currentStrategy()->applyImpulse(impulse, r1);
    c2->currentStrategy()->applyImpulse(V2Extend::Negative(impulse), r2);

//    j = -std::abs(j) * 0.01;
//    qreal kMass = c1->mass() * c2->inv_mass();
//    qreal damage2 = kMass * j;
//    qreal damage1 = j - damage2; //(1 - kMass) * j
//    c1->changeHealth(static_cast<int>(damage1));
//    c2->changeHealth(static_cast<int>(damage2));

}
