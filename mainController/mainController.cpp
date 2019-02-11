#include "mainController.h"
#include "mainController/optionsDialog/gamesettings.h"
#include "mainController/startDialog/startdialog.h"
#include "gameWidget/widget.h"
#include <QMessageBox>
#include <QDebug>

MainController::MainController(QObject *parent):
    QObject(parent)
{
    GameSettings::getSingle().loadLanguage();
}

void MainController::createStartDialog()
{
    startD = std::make_unique<startDialog>();
    connect(startD.get(), SIGNAL(signalStartGame(QString)), this, SLOT(slotCreateMainWidget(QString)));
    connect(startD.get(), SIGNAL(signalEndProgramm()), this, SLOT(slotEndProgramm()));
    startD->show();
}

void MainController::slotCreateMainWidget(QString pixmapPath)
{
    startD->setVisible(false);
    widget = std::make_unique<MainWidget>(pixmapPath);
    connect(widget.get(), SIGNAL(signalEndOfStage(int)), this, SLOT(slotSaveScore(int)));
    connect(widget.get(), SIGNAL(signalEndOfGame()), this, SLOT(slotEndGame()));

    widget->show();
}

void MainController::slotEndProgramm()
{

}

void MainController::slotEndGame()
{
    widget.reset();
    startD->setVisible(true);
}

//пока не конечный вариант
void MainController::slotSaveScore(int score)
{
    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);
    msg.setText(tr("You get: ") + QString::number(score) + tr(" score!!!"));
    msg.setWindowTitle(tr("Your Score!!!"));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();

    widget->close();
}


MainController::~MainController()
{
    qDebug()<<"End of MainController";
}
