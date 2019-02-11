#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

#include <QGraphicsView>

class CustomView: public QGraphicsView
{
public:
    CustomView(QGraphicsScene* scene, QWidget* parent = nullptr);
    void setBoardView(int b);
    void update();
private:
    int boardView;
};

#endif // CUSTOMVIEW_H
