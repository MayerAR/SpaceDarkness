#include "singlecursor.h"
#include <QCursor>


//cursor
cursorDirection* cursorDirection::p_single = nullptr;

cursorDirection::~cursorDirection()
{
    this->deleteClass();
}

void cursorDirection::changeCursorPosition(const QPoint &newPosition)
{
    if(QCursor::pos() == newPosition)
        return;
    emit signalChangeCursorPosition(newPosition);
}

void cursorDirection::deleteClass()
{
    delete p_single;
}

cursorDirection *cursorDirection::getCursor()
{
    if(p_single == nullptr)
        p_single = new cursorDirection();
    return p_single;
}

