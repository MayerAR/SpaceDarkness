#include "controller.h"
#include "makeMove.h"
#include "gameWidget/customScene/customscene.h"
#include "gameWidget/customScene/customview.h"
#include "gameWidget/loadStage/loadStage.h"
#include "readXmlFiles/readfile.h"
#include "gameWidget/objects/differentObjects/gameobject.h"
#include "gameWidget/mainItem/mainItem.h"
#include "gameWidget/objects/differentObjects/AllObjectParam/allobjectparam.h"
#include "gameWidget/objects/differentObjects/WeaponParam/weaponparam.h"
#include "gameWidget/widget.h"
#include <QDebug>
#include <ctime>
#include <QTimer>


Controller::Controller(const QString& stagePath, MainWidget *parent):
    curScene(new CustomScene(parent)),
    curView(new CustomView(curScene.get(), parent)),
    data(nullptr),
    loadWave(nullptr),
    controllStep(new makeMove()),
    frameTimer(new QTimer())
{


    srand(unsigned(std::time(nullptr)));

    this->readStage(stagePath);

    connect(frameTimer.get(), &QTimer::timeout, this, &Controller::slotUpdate);
    frameTimer->setInterval(static_cast<int>(timeStamp * 1000));
    frameTimer->start();

    connect(this, &Controller::signalChangeCursor, parent, &MainWidget::slotChangeCursorPosition);

}

Controller::~Controller()
{
    pauseGame();
    controllStep.reset();
    curScene.reset();
    createObject::getSingle()->deleteSingle();
    //удаляем данные из singletone классов
    AllObjectParam::deleteParam();
    AllWeaponParam::deleteParam();
}

QPointF Controller::topLeftView() const
{
    return curView->mapToScene(QPoint(0, 0));
}

void Controller::pauseGame()
{
    frameTimer->stop();
}

void Controller::continueGame()
{
    frameTimer->start();
}

//setup curScene and curView
void Controller::setCustomScene(std::shared_ptr<xmlElem> optionsScene)
{    
    createObject::getSingle()->setScale(optionsScene->nextLevel.value("scale")->value.toDouble());

    auto width = optionsScene->nextLevel.value("width")->value.toDouble();
    auto height = optionsScene->nextLevel.value("height")->value.toDouble();
    QPixmap bg(optionsScene->nextLevel.value("BgPath")->value);
    curScene->setBackgroundBrush(QBrush(QColor(255, 255, 255), bg));
    curScene->setSceneRect(0, 0, width, height);

    controllStep->addBorders(QPointF(-1, height/2), QSize(15, height), curScene);
    controllStep->addBorders(QPointF(width + 1, height/2), QSize(15, height), curScene);
    controllStep->addBorders(QPointF(width/2, -1), QSize(width, 15), curScene);
    controllStep->addBorders(QPointF(width/2, height + 1), QSize(width, 15), curScene);


    auto mainItemPtr = MainItem::getMainItem();
    connect(curScene.get(), &CustomScene::signalKeyPress, mainItemPtr, &MainItem::slotKeyPress);
    connect(curScene.get(), &CustomScene::signalKeyRelease, mainItemPtr, &MainItem::slotKeyRelease);
    connect(curScene.get(), &CustomScene::signalMouseMove, mainItemPtr, &MainItem::slotMouseMove);
    connect(curScene.get(), &CustomScene::signalLeftMousePress, mainItemPtr, &MainItem::slotMousePress);
    connect(curScene.get(), &CustomScene::signalLeftMouseRelease, mainItemPtr, &MainItem::slotMouseRelease);

    curView->setFixedSize(optionsScene->nextLevel.value("widthView")->value.toInt(),
                                optionsScene->nextLevel.value("heightView")->value.toInt());

    curView->setBoardView(optionsScene->nextLevel.value("boardView")->value.toInt());


}

void Controller::slotUpdate()
{
    if(!controllStep->haveEnemies())
    {
        nextWave();
        return;
    }
    //calculate center of focus view
    curView->update();
    controllStep->nextStep(curScene.get());
}

void Controller::restartGame()
{
    loadWave->restartWaves();
    controllStep->addNewWave(loadWave->nextWave(), curScene);

    MainItem::getMainItem()->restartItem();
    frameTimer->start();
}

void Controller::slotDestroedMainItem()
{
    frameTimer->stop();
    emit signalDestroedMainItem();
}

void Controller::slotChangeCursor(QPoint cur)
{
    emit signalChangeCursor(cur);
}

void Controller::readStage(const QString& stagePath)
{
    data = readXmlFile(stagePath);

    auto stageXml = data->nextLevel.value("stage");
     if(!findInXmlMap(stageXml, "options_map"))  //check if all settings available in current xml file
        if(!findInXmlMap(stageXml, "waves"))
            if(!findInXmlMap(stageXml, "mainShip"))
                exit(40);

    setCustomScene(stageXml->nextLevel.value("options_map")); //setup currScene and curView

    loadWave = std::make_shared<LoadWave>(stageXml->nextLevel.value("waves")); //load enemy items
    nextWave(); //load current wave on scene

    createMainShip(stageXml->nextLevel.value("mainShip")); //load mainShip

}

void Controller::createMainShip(std::shared_ptr<xmlElem> xmlMain)
{
    int x = xmlMain->nextLevel.value("x_pos")->value.toInt();
    int y = xmlMain->nextLevel.value("y_pos")->value.toInt();

    MainItem::getMainItem()->getObject()->addToScene(curScene.get());
    MainItem::getMainItem()->getObject()->setPosition(QPointF(x, y));
    connect(MainItem::getMainItem(), &MainItem::signalDestroedItem, this, &Controller::slotDestroedMainItem);
    connect(MainItem::getMainItem(), &MainItem::signalChangeCursor, this, &Controller::slotChangeCursor);
}

void Controller::nextWave()
{

    if(!loadWave->ifNextWave())
    {
        frameTimer->stop();
        emit signalEndOfStage(1000);
        return;
    }

    controllStep->addNewWave(loadWave->nextWave(), curScene);
}








