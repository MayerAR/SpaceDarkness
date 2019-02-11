#ifndef SINGLECURSOR_H
#define SINGLECURSOR_H
#include <QObject>


//class that send signal to current widget to change cursor position on screen [not on widget, but on screen]
class cursorDirection: public QObject
{
    Q_OBJECT
private:
    static cursorDirection* p_single;
public:
    ~cursorDirection();
    cursorDirection(QObject* parent = nullptr): QObject(parent) {}
    void changeCursorPosition(const QPoint& newPosition);
    static void deleteClass();
    static cursorDirection* getCursor();
signals:
    void signalChangeCursorPosition(const QPoint&);
};


#endif // SINGLECURSOR_H
