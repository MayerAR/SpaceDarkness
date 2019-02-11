#include "widget.h"
#include "systemItem/singlecursor.h"
#include <QKeyEvent>
#include "controller/controller.h"
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>

MainWidget::MainWidget(const QString &stagePath, QWidget *parent) :
    QWidget(parent),
    contr(new Controller(stagePath, this))
{
    connect(contr.get(), SIGNAL(signalEndOfStage(int)), this, SLOT(slotEndOfStage(int)));
    connect(contr.get(), SIGNAL(signalDestroedMainItem()), this, SLOT(slotDestroedMainItem()));

    this->setMinimumSize(960, 540);


//    connect(cursorDirection::getCursor(), SIGNAL(signalChangeCursorPosition(const QPoint&)), this, SLOT(slotChangeCursorPosition(const QPoint&)));

}

MainWidget::~MainWidget()
{
}

void MainWidget::slotChangeCursorPosition(const QPoint& newSceneMousePos)
{
    QPointF zeroView = contr->topLeftView();
    QPoint newViewPos = newSceneMousePos - QPoint(static_cast<int>(zeroView.x()), static_cast<int>(zeroView.y()));

    QCursor::setPos(this->mapToGlobal(newViewPos));
}

void MainWidget::pauseWidget()
{
    contr->pauseGame();
    QMessageBox msg;
    msg.setText(tr("Pause of a game!"));

    QPushButton* exitPushButton = msg.addButton(tr("Exit"), QMessageBox::ActionRole);
    msg.addButton(tr("Continue"), QMessageBox::RejectRole);
    msg.exec();
    if(msg.clickedButton() == exitPushButton)
        this->close();
    else
        contr->continueGame();
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{

    if(event->key() == Qt::Key_Escape)
        pauseWidget();
    else
        QWidget::keyPressEvent(event);

}

void MainWidget::closeEvent(QCloseEvent */*event*/)
{
    contr.reset();
    emit signalEndOfGame();
}

void MainWidget::slotEndOfStage(int score)
{
    emit signalEndOfStage(score);
}

void MainWidget::slotDestroedMainItem()
{
    QMessageBox msg;
    msg.setText(tr("End of a game!"));


    msg.addButton(tr("Exit"), QMessageBox::RejectRole);
    QPushButton* restartPushButton = msg.addButton(tr("Restart"), QMessageBox::ActionRole);
    msg.exec();
    if(msg.clickedButton() != restartPushButton)
        emit signalEndOfGame();
    else
        contr->restartGame();



}




