#include "customview.h"
#include "gameWidget/mainItem/mainItem.h"


CustomView::CustomView(QGraphicsScene *scene, QWidget *parent):
    QGraphicsView(scene, parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMouseTracking(true);

}

void CustomView::setBoardView(int b)
{
    boardView = b;
}

void CustomView::update()
{
    const QPointF mainPosition = MainItem::getMainItem()->positionMainShip();
    const QPointF oldPositionView = this->mapToScene(this->pos());
    const QSize sizeView(this->size());
    qreal centerX, centerY;

    if(mainPosition.x() > oldPositionView.x() + sizeView.width() - boardView)
        centerX = mainPosition.x() +boardView - sizeView.width()/2;
    else if (mainPosition.x() < oldPositionView.x()+ boardView)
        centerX = mainPosition.x() - boardView + sizeView.width()/2;
    else
        centerX = sizeView.width()/2 + oldPositionView.x() - 1;

    if(mainPosition.y() < oldPositionView.y() + boardView)
        centerY = mainPosition.y() - boardView + sizeView.height()/2;
    else if (mainPosition.y() > oldPositionView.y() + sizeView.height() - boardView)
        centerY = mainPosition.y() + boardView - sizeView.height()/2;
    else
        centerY = sizeView.height()/2 + oldPositionView.y()-1;


    centerOn(centerX, centerY); //move this view if mainShip is out of view
}
