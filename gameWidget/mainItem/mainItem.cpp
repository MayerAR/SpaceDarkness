#include "mainItem.h"
#include "gameWidget/loadStage/loadStage.h"
#include "gameWidget/objects/differentObjects/gameobject.h"
#include "gameWidget/strategy/InterfaceStrategy.h"
#include "gameWidget/objects/differentObjects/AllObjectParam/allobjectparam.h"
#include "gameWidget/objects/changeobjectdata.h"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>
#include <QMap>

MainItem* MainItem::mItem = nullptr;
//class MainItem
MainItem::MainItem()
{
    mainShip = createObject::getSingle()->createMainItem();
}

MainItem::~MainItem()
{
    mainShip.reset();
}

MainItem *MainItem::getMainItem()
{
    if(mItem == nullptr)
        mItem = new MainItem;

    return mItem;
}

void MainItem::deleteObject()
{
    delete mItem;
    mItem = nullptr;
}

void MainItem::restartItem()
{
    mainShip->resetHealth();
    mainShip->resetShot();
    mainShipVelocity.clear();
    mainShipBullet = false;

}

std::shared_ptr<GameObject> MainItem::getObject()
{
    return mainShip;
}

void MainItem::changeCursor()
{
    QPointF scenePosition = mainShip->pixmapItem()->mapToScene(QPointF(0, -mainShip->pixmapItem()->pixmap().rect().height() * 2)); //calculate new position of cursor
    emit signalChangeCursor(QPoint(static_cast<int>(scenePosition.x()), static_cast<int>(scenePosition.y()))); //change cursor Position
}

Vector2D MainItem::velocityMainShip() const
{
    return mainShip->curVelocity();
}

QPointF MainItem::positionMainShip() const
{

    return mainShip->pixmapItem()->pos();
}

qreal MainItem::updateAngleMainShip()
{
    auto res = calculateAngle(QPointF(0, 0), mainShip->pixmapItem()->mapFromScene(oldPosMouse));

//    if(res < 0.5 && res > -0.5)
//        return;

    return res;
}

Vector2D MainItem::updateVelocityMainShip()
{
    Vector2D deltaV(0, 0);
    qreal curAcceleration = AllObjectParam::getSingle()->currentParam(mainShip->idObject())->acceleration;
    if(mainShipVelocity[Qt::Key_A] == true)
        deltaV.setX(- curAcceleration);

    if(mainShipVelocity[Qt::Key_D] == true)
        deltaV.setX(deltaV.x() + curAcceleration);

    if(mainShipVelocity[Qt::Key_W] == true)
        deltaV.setY(deltaV.y() - curAcceleration);

    if(mainShipVelocity[Qt::Key_S] == true)
        deltaV.setY(deltaV.y() + curAcceleration);

    return deltaV;
}

bool MainItem::calculateNextStep()
{
    if(!mainShip->ifAlive())
    {
        emit signalDestroedItem();
        return false;
    }

//    mainShip->changeHealth(-100);


    auto changeData = mainShip->currentStrategy()->nextStep(); //if our item is on scene go to mainShip strategy and calculate deltaPos and deltaVelocity
    if(changeData->angleRad == 0.0) //when this is true we are bumb, because we don't controll current ship
    {
        changeData->deltaVelocity = updateVelocityMainShip();
        changeData->angleRad = updateAngleMainShip();
    }
    else
        changeCursor();

    mainShip->update(changeData);
    bool createBullet = mainShipBullet && mainShip->couldShot();

//    return true;
    return createBullet;
}

void MainItem::slotKeyPress(int key_id)
{
    mainShipVelocity[key_id] = true; //save in QMap all keys, that been press
}

void MainItem::slotKeyRelease(int key_id)
{
    mainShipVelocity[key_id] = false; //remove from QMap [mainShipVelocity] all keys, that been release
}

void MainItem::slotMouseMove(QPointF newPosMouse)
{
    oldPosMouse = newPosMouse;
//    updateAngleMainShip(); //if we move mouse --> update cur rotation of mainItem
                            //and save current position of mouse
                                //and if we dont move mouse -- our oldPosMouse will be change rotation of mainItem
}

void MainItem::slotMousePress()
{
    mainShipBullet = true;
//    qDebug()<<"slotMousePress";
//    controllStep->createBulletOfShip(MainItem::getMainItem()->getObject(), curScene);
}

void MainItem::slotMouseRelease()
{
    mainShipBullet = false;
}
