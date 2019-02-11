#ifndef MAKEMOVE_H
#define MAKEMOVE_H

#include <memory>
#include <QList>

class QGraphicsScene;
class GameObject;
class BulletObject;
class Object;
class BorderObject;

using EnemyList = QList<std::shared_ptr<GameObject>>;
using BulletList = QList<std::shared_ptr<BulletObject>>;
using BorderList = QList<std::shared_ptr<BorderObject>>;

class makeMove
{
public:
    makeMove();
    ~makeMove();

    void addNewWave(EnemyList newWave, std::shared_ptr<QGraphicsScene> scene); //update enemy objects on scene
    void nextStep(QGraphicsScene *scene);
    bool haveEnemies() const;
    void addBorders(const QPointF& center, const QSize& size, std::shared_ptr<QGraphicsScene> scene);
private:
    bool findCollideBullets(std::shared_ptr<BulletObject> b);

    //calculate step
    void calculateNextStep(std::shared_ptr<GameObject> curItem);
    void calculateNextStep(std::shared_ptr<BulletObject> curBullet);
    void createBulletOfShip(const std::shared_ptr<GameObject> curShip, QGraphicsScene* curScene);


    bool Collide(std::shared_ptr<Object> i1, std::shared_ptr<Object> i2);
    bool Collide(std::shared_ptr<GameObject> i, std::shared_ptr<BulletObject> b);
    bool avaidBumb(std::shared_ptr<Object> obj1, std::shared_ptr<Object> obj2);

    //delete elem
    void deleteElemFromList(std::shared_ptr<GameObject> curItem);
    void deleteElemFromList(std::shared_ptr<BulletObject> curItem);

    //add new wave on scene
    void addWaveOnScene(const std::shared_ptr<QGraphicsScene> scene) const; //add all enemy items of current wave on scene
    void deleteCurrentWave();

    BorderList borders;
    EnemyList curWave;
    BulletList bullets;

};

#endif // MAKEMOVE_H























