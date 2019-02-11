#include "choosestagewidget.h"
#include "ui_choosestagewidget.h"
#include "readXmlFiles/readfile.h"
#include <QMessageBox>
//#include <QTextCodec>
#include <memory>
#include <QtGui>
#include <QDebug>

ChooseStageWidget::ChooseStageWidget(const QString &allStagesPath_, QWidget *parent) :
    QDialog(parent),
    allStagesPath(allStagesPath_),
    ui(new Ui::ChooseStageWidget)
{
    ui->setupUi(this);


    updateComboBox();


}

ChooseStageWidget::~ChooseStageWidget()
{
    delete ui;
}

void ChooseStageWidget::updateComboBox()
{
    auto currentStages = readXmlFile(allStagesPath)->nextLevel.value("stages");
    for(const auto& elem: currentStages->nextLevel)
        ui->comboBox->addItem(elem->name,
                              QVariant(elem->nextLevel.value("resource")->value));
}

//slots
void ChooseStageWidget::on_okeyButton_clicked()
{
    emit signalCreateNewStage(ui->comboBox->currentData().toString());
    this->accept();
}

