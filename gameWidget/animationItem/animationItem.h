#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>

class QTimer;

//if our item are delete we call this animation class
class EndingAnimation:public QObject,  public QGraphicsPixmapItem
{
    Q_OBJECT
private:
    ~EndingAnimation() override;
public:
    EndingAnimation(QGraphicsItem* parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setBoundingRect(qreal width, qreal height);
    void start();

private slots:
    void next_frame();
public:
    int mFrames = 0; //how much frames
    int mWidth = 0; //width of one frame
    int mHeight = 0;//height of one frame
    int mOffset = 0; // offset of one frame
    int mBorder = 0; // border behind the frames
    int mTimeOut = 0; //frequency of animation
    QPixmap* mPixmap; //all frames

private:
    int mCurrentFrame = 0;

    QRectF bRect; //bounding Rect of animation
    QTimer* mTimer;

};



//make text move
class TextAnimation:public QGraphicsTextItem
{
    Q_OBJECT
private:
    ~TextAnimation() override;
public:
    TextAnimation(const QString &text, QGraphicsItem* parent = nullptr); //create object that depend of text
    void start();


private slots:
    void next_frame(); //create next frame of cur animation


private:
    int numbOfFrames = 10;
    QTimer* mTimer;
};


#endif // ANIMATION_H
