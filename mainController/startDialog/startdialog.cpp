#include "startdialog.h"
#include "ui_startdialog.h"
#include "mainController/optionsDialog/optionsdialog.h"
#include "mainController/chooseStageWidget/choosestagewidget.h"
#include <QMessageBox>

startDialog::startDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::startDialog)
{
    ui->setupUi(this);

}

startDialog::~startDialog()
{
    delete ui;
}

void startDialog::closeEvent(QCloseEvent */*event*/)
{
    emit signalEndProgramm();
}

void startDialog::on_startPushButton_clicked()
{
    std::unique_ptr<ChooseStageWidget> widget(new ChooseStageWidget(":/stage/allStages.xml", this));
    connect(widget.get(), SIGNAL(signalCreateNewStage(QString)), this, SLOT(slotCreateStage(QString)));
//    widget->show();
    widget->exec();
}

void startDialog::on_optionsPushButton_clicked()
{
    optionsDialog opt;
    opt.exec();
}

void startDialog::on_keysPushButton_clicked()
{

}

void startDialog::on_leaderPushButton_clicked()
{

}

void startDialog::on_aboutPushButton_clicked()
{

    QMessageBox::about(this,tr("About SpaceDarkness"),tr("<h2>SpaceDarkness 1</h2>"
                                                        "<p>SpaceDarkness is a small application that"
                                                        "demonstrates QCustomScene, QCustomView, QML,"
                                                        " siganls & slots and many other Qt classes "
                                                        "<p> Mayer A. R. 2019"
                                                         ));
}

void startDialog::on_aboutQtPushButton_clicked()
{
    qApp->aboutQt();
}

void startDialog::slotCreateStage(QString pixmapPath)
{
    emit signalStartGame(pixmapPath);
}

