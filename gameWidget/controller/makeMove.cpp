#include "makeMove.h"
#include "gameWidget/strategy/InterfaceStrategy.h"
#include "gameWidget/loadStage/loadStage.h"
#include "gameWidget/objects/allobjects.h"
#include "gameWidget/objects/changeobjectdata.h"
#include "gameWidget/contactItems/contact.h"
#include "gameWidget/mainItem/mainItem.h"
#include "gameWidget/objects/differentObjects/borderobject.h"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>
#include <QVector>

//Create all Elements...
makeMove::makeMove()
{
    //    mainShip = nullptr;
}

makeMove::~makeMove()
{
    bullets.clear();
    //удаляем данные mainItem -- в singletone классе MainItem остается только указатель на nullptr
    MainItem::deleteObject();

    //delete EnemyList and BulletList
    //we reset all of this objects
    //all of them must be delete
}

void makeMove::addNewWave(EnemyList newWave, std::shared_ptr<QGraphicsScene> scene)
{
    //if we have objects on scene -- we must delete this!
    if(!curWave.empty())
    {
        deleteCurrentWave();
    }
    //copy
    curWave = newWave;

    //add on scene
    addWaveOnScene(scene);
}


void makeMove::nextStep(QGraphicsScene * scene)
{
    for(auto elem: bullets) //calculate next position for all bullets
        calculateNextStep(elem);


    for(auto item1 = curWave.begin(), end = curWave.end(); item1 != end; item1++)
    {
        for(auto item2 = item1 + 1; item2 < end; item2++)
            if(item1 != item2)
                if((*item1)->pixmapItem()->collidesWithItem((*item2)->pixmapItem().get())) //check boundingRect
                    Collide(*item1, *item2); // check item1 and item2 about colliding pixmapRect

        for(auto& elem: borders)
            if((*item1)->pixmapItem()->collidesWithItem(elem->pixmapItem().get())) //check boundingRect
                Collide(*item1, elem); // проверка столкновений между Границей и Элемнтами

        if((*item1)->pixmapItem()->collidesWithItem(MainItem::getMainItem()->getObject()->pixmapItem().get())) //check bounding rect
            Collide(*item1, MainItem::getMainItem()->getObject()); //check item1 and mainShip about colliding

        calculateNextStep(*item1); //change pos of item1
    }

   QVector<std::shared_ptr<BulletObject>> destroedBullet; //create vector that save all bullets, that we must destroed in current step
    for(auto bulletIterator = bullets.begin(), end = bullets.end();
        bulletIterator != end;
        bulletIterator++)
    {

        if((*bulletIterator)->pixmapItem()->collidesWithItem(MainItem::getMainItem()->getObject()->pixmapItem().get()))
            if(Collide(MainItem::getMainItem()->getObject(), *bulletIterator))
            {
                destroedBullet.push_back(*bulletIterator);
                continue;
            }

        if(findCollideBullets(*bulletIterator)) //current bullet are delete...
            destroedBullet.push_back(*bulletIterator); //we can't destroed it now, because if we destroed it now our bullets.begin(), bullets.end() and iterator bullet will be changed

    }


    for(auto& elem: destroedBullet) //destroed all elems of our vector
        deleteElemFromList(elem); //forget about current objects --> now only local variable has got shared_ptr of this objects and they delete soon
    destroedBullet.clear();


    for(auto& elem: borders)
    {
        if(MainItem::getMainItem()->getObject()->pixmapItem()->collidesWithItem(elem->pixmapItem().get())) //check boundingRect
        {
            Collide(MainItem::getMainItem()->getObject(), elem); // check item1 and item2 about colliding pixmapRect
        }
    }

    if(MainItem::getMainItem()->calculateNextStep())
    {
        createBulletOfShip(MainItem::getMainItem()->getObject(), scene);
        MainItem::getMainItem()->getObject()->fixShot();
    }
}

bool makeMove::haveEnemies() const
{
    return curWave.size() != 0;
}

void makeMove::addBorders(const QPointF &center, const QSize &size, std::shared_ptr<QGraphicsScene> scene)
{
    borders.push_back(std::shared_ptr<BorderObject>(new BorderObject("", size, objectID::Border, strategyID::BaseStrategy)));
    scene->addItem(borders.back()->pixmapItem().get());
    borders.back()->setPosition(center);
}

//find collisions with different objects
bool makeMove::findCollideBullets(std::shared_ptr<BulletObject> b)
{
    qreal minDist = std::numeric_limits<qreal>::max();
    std::shared_ptr<GameObject> minObject(nullptr);
    QPointF posBullet = b->pixmapItem()->pos();

    for(auto itemIterator = curWave.begin(), end = curWave.end();
        itemIterator != end;
        itemIterator++)
    {
        if(b->pixmapItem()->collidesWithItem(
                    (*itemIterator)->pixmapItem().get()))
        {
            Vector2D vD = posBullet - (*itemIterator)->pixmapItem()->pos();
            qreal curDist = vD.x() * vD.x() + vD.y() * vD.y();
            if(minDist > curDist)
            {
                minDist = curDist;
                minObject = *itemIterator;
            }
        }
    }
    if(minObject == nullptr)
        return false;

    return Collide(minObject, b);

}


//for enemy object ( not for MainItem)
void makeMove::calculateNextStep(std::shared_ptr<GameObject> curItem)
{
    if(!curItem->ifAlive())
    {
        deleteElemFromList(curItem);//if it < 0 --> delete this Item
        return;
    }

//    createBulletOfShip(curItem, curItem->pixmapItem->scene());
    auto changeData = curItem->currentStrategy()->nextStep(); //if our item is on scene go to curItem strategy and calculate deltaPos and deltaVelocity
    if(changeData->ifShot && curItem->couldShot()) //if( A && B ): A -- true, when currentStrategy think that we can do shot. B -- true if curWeapon could do shot
    {
        //detect shot
        createBulletOfShip(curItem, curItem->pixmapItem()->scene()); //create bullet
        curItem->fixShot(); //say that we create shot
    }
    curItem->update(changeData);

}

void makeMove::calculateNextStep(std::shared_ptr<BulletObject> curBullet)
{
    //if bullet is beyond the boundaries of the scene
    if(!curBullet->ifAlive())
    {
        deleteElemFromList(curBullet); //if bullet out of scene
        return;
    }

    //if bullet on scene --> go to bullet's strategy
    curBullet->update(curBullet->currentStrategy()->nextStep());// change position of bullet.
}


void makeMove::createBulletOfShip(const std::shared_ptr<GameObject> curShip, QGraphicsScene* curScene)
{
    auto bullet = createObject::getSingle()->createBullet(curShip);
    bullet->addToScene(curScene);
    bullets.push_back(bullet);
}

bool makeMove::Collide(std::shared_ptr<Object> i1, std::shared_ptr<Object> i2)
{
    if(avaidBumb(i1, i2))
        return Contact::bumb(i1, i2); //Contact::bumb(shared_ptr<GameObject>, shared_ptr<GameObject>)

    return false;
}

bool makeMove::Collide(std::shared_ptr<GameObject> i, std::shared_ptr<BulletObject> b)
{
    if(avaidBumb(i, b))
        return Contact::bumb(b, i);  //Contact::bumb(shared_ptr<GameObject>, shared_ptr<BulletObject>)

    return false;
}

//avoid duplicate code
bool makeMove::avaidBumb(std::shared_ptr<Object> obj1, std::shared_ptr<Object> obj2)
{
    QPolygonF first( obj1->localPixmapRect());
    QPolygonF second( obj2->localPixmapRect());
    first = obj1->pixmapItem()->mapToItem(obj2->pixmapItem().get(), first);
    if(first.intersects(second))
        return true;

    obj1->currentStrategy()->avaidBumbWithObject(obj2);
    obj2->currentStrategy()->avaidBumbWithObject(obj1);
    return false;
}


//we forgot about curItem
void makeMove::deleteElemFromList(std::shared_ptr<GameObject> curItem)
{
    auto iter = curWave.begin(), end = curWave.end();

    //check if our object is from QList curWave
    for(; iter != end; iter++)
    {
        if(*iter == curItem)
        {
            curWave.erase(iter); //yes it's from list --> delete this ceil
            break;
        }
    }
    if(iter == end)
    {
        //no it's not from QList --> it's error send warning message
        qDebug()<<"this elem not from current wave! void makeMove::deleteElem(GameObject *curItem)";
        system("pause");
        return;
    }
}


// we forget current object
//and we delete it soon
void makeMove::deleteElemFromList(std::shared_ptr<BulletObject> curItem)
{
    auto iter = bullets.begin(), end = bullets.end();

    //if we need animation of destroyed we must create new object...
//    curItem->pixmapItem->scene()->removeItem(curItem->pixmapItem);
    for(; iter != end; iter++)
    {

        if(*iter == curItem)
        {
            bullets.erase(iter); //erase shared from vector
            break;
        }
    }
    if(iter == end)
    {
        //no it's not from QList --> it's error send warning message
        qDebug()<<"this elem not from current wave! void makeMove::deleteElem(std::shared_ptr<BulletObject> curItem)";
        system("pause");
        return;
    }
}

//add all of objects on scene
void makeMove::addWaveOnScene(const std::shared_ptr<QGraphicsScene> scene) const
{
    for(auto& elem: curWave)
    {
        scene->addItem(elem->pixmapItem().get());
    }

//    elem->addToScene(scene);
}

//delete all enemy objects
void makeMove::deleteCurrentWave()
{
    qDebug()<<"delete current wave...";
    for(auto& elem: curWave)
        deleteElemFromList(elem); //delete from list --> shared_ptr<...> are reset and objects will be delete
    for(auto& elem: bullets)
        deleteElemFromList(elem);
}



