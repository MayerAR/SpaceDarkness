#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <memory>
#include <QPoint>

struct xmlElem;
class makeMove;
class CustomScene;
class CustomView;
class LoadWave;
class MainWidget;
class QTimer;


class Controller: public QObject
{
    Q_OBJECT
public:
    Controller(const QString& stagePath, MainWidget *parent = nullptr);
    ~Controller(); //when our game is closed

    //return topLeft point of view(on scene)
    //need for chage cursor position
    QPointF topLeftView() const;

    void pauseGame();
    void continueGame();

    void restartGame();
private slots:
    //slot from timer
    void slotUpdate();
    //slot from MainItem(if our item is destroed)
    void slotDestroedMainItem();
    void slotChangeCursor(QPoint);
signals:
    //ifNextWave = false
    void signalDestroedMainItem();
    void signalEndOfStage(int);
    void signalChangeCursor(QPoint);
private:
    //read file stagePath, and create wave and mainItem
    void readStage(const QString &stagePath);
    void setCustomScene(std::shared_ptr<xmlElem> optionsScene); //setup curScene and curView
    void createMainShip(std::shared_ptr<xmlElem> xmlMain); //read xmlMain to create mainItem
    void nextWave(); //update objects on scene in controllStep
    const QPointF newPositionOfView() const;

    std::shared_ptr<CustomScene> curScene;
    std::shared_ptr<CustomView> curView;

    std::shared_ptr<xmlElem> data;//shared_ptr to xml file of stage
    std::shared_ptr<LoadWave> loadWave;  //loading current wave
    std::shared_ptr<makeMove> controllStep;//calculate next step of all items
    std::shared_ptr<QTimer> frameTimer; //controll ticks of current stage

    int boardView; // distance from mainShip to boarder

};


#endif // CONTROLLER_H
