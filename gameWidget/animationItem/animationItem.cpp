#include "animationItem.h"

//#include <QFont>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>



//animation
EndingAnimation::EndingAnimation(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    mTimer = new QTimer;
    connect(mTimer, &QTimer::timeout, this, &EndingAnimation::next_frame);
}

EndingAnimation::~EndingAnimation()
{
   //delete item from scene and delete all pointers
    if(this->scene())
        this->scene()->removeItem(this);
    delete mTimer;
    delete mPixmap;
}

//draw cur frame of our animation
void EndingAnimation::paint(QPainter *painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
    QPixmap curFrame = mPixmap->copy((mOffset * mCurrentFrame + mBorder), 0, mWidth, mHeight).scaled(static_cast<int>(bRect.width()),
                                                                                                     static_cast<int>(bRect.height())); //save in QPixmap our frame

    painter->drawPixmap(static_cast<int>(bRect.topLeft().x()),
                        static_cast<int>(bRect.topLeft().y()),
                        curFrame);
}

void EndingAnimation::setBoundingRect(qreal width, qreal height)
{
    bRect.setRect(-width/2, -height/2, width, height);
}

void EndingAnimation::start()
{
    mTimer->setInterval(mTimeOut);
    mTimer->start();
}

void EndingAnimation::next_frame()
{
    mCurrentFrame++;
    if(mCurrentFrame >= mFrames)
    {
        mTimer->stop();
        delete this;
        return;
    }
    QGraphicsItem::update();

}

QRectF EndingAnimation::boundingRect() const
{
    return bRect;
}


TextAnimation::TextAnimation(const QString& text, QGraphicsItem *parent):QGraphicsTextItem(text, parent) //create text item
{
    QFont font("Helvetica", -1, -1); //set parametres of cur item
    this->setFont(font);
    this->setDefaultTextColor(QColor(0, 255, 0));

    mTimer = new QTimer; //create Timer
    connect(mTimer, &QTimer::timeout, this, &TextAnimation::next_frame);
}

TextAnimation::~TextAnimation()
{
    if(this->scene())
        this->scene()->removeItem(this);
    delete mTimer;
}

void TextAnimation::start()
{
    mTimer->setInterval(50);
    mTimer->start();
}

void TextAnimation::next_frame()
{
    this->moveBy(0, -3);
    numbOfFrames--;
    if(numbOfFrames == 0)
    {
        mTimer->stop();
        delete this;
    }
}
