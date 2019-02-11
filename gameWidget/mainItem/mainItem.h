#ifndef MAINITEM_H
#define MAINITEM_H

#include <memory>
#include <QObject>
#include <QMap>
#include "gameWidget/constants.h"

class GameObject;

class MainItem: public QObject
{
    Q_OBJECT
private:
    static MainItem* mItem;
    std::shared_ptr<GameObject> mainShip;

    QMap<int, bool> mainShipVelocity; //save current keys that was be pressed, for synchronous update mainShip velocity
    bool mainShipBullet = false;
    QPointF oldPosMouse; //save

public:
    MainItem();
    ~MainItem();
    static MainItem* getMainItem();
    static void deleteObject();
    void restartItem();
    std::shared_ptr<GameObject>  getObject(); //we can change current mainShip

    Vector2D velocityMainShip() const;
    QPointF positionMainShip() const;
    bool calculateNextStep(); //return true if we can do shot
public slots:
    //slots from curScene
    void slotKeyPress(int key_id);
    void slotKeyRelease(int key_id);
    void slotMouseMove(QPointF newPosMouse);
    void slotMousePress();
    void slotMouseRelease();
signals:
    void signalChangeCursor(QPoint pp); //maybe yes maybe no
    void signalDestroedItem(); //emit signal when current health < 0

private:
    void changeCursor(); //change cursor of mainShip when mainShip is bumb
    qreal updateAngleMainShip();
    Vector2D updateVelocityMainShip();
};

#endif // MAINITEM_H
