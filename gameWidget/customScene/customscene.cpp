#include "customscene.h"
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsItem>

CustomScene::CustomScene(QObject *parent): QGraphicsScene(parent)
{
}

CustomScene::~CustomScene()
{
}


void CustomScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_A:
    case Qt::Key_D:
    case Qt::Key_S:
    case Qt::Key_W:
        emit signalKeyPress(event->key()); //save key's in QMap
        break;
    default:
        QGraphicsScene::keyPressEvent(event);
        break;
    }
}

void CustomScene::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_A:
    case Qt::Key_D:
    case Qt::Key_S:
    case Qt::Key_W:
        emit signalKeyRelease(event->key()); //remove key's from QMap
        break;
    default:
        break;
    }
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        emit signalLeftMousePress();
}

void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        emit signalLeftMouseRelease();
}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit signalMouseMove(event->scenePos());
}


