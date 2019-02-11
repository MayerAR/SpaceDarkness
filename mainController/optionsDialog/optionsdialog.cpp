#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "mainController/optionsDialog/gameoptionstab.h"

optionsDialog::optionsDialog(QWidget *parent) :
    QDialog(parent),

    ui(new Ui::optionsDialog)
{    
    ui->setupUi(this);
    auto firstTab = new GameOptionsTab(this);
    ui->tabWidget->addTab(firstTab, tr("Base Settings"));
    connect(firstTab, SIGNAL(signalChanged()), this, SLOT(changeDialog()));
    connect(this, SIGNAL(signalSaveChanges()), firstTab, SLOT(acceptCurrentChanges()));

    auto secondTab = new ObjectOptionsTab(this);
    ui->tabWidget->addTab(secondTab, tr("Objects Settings"));
    connect(secondTab, SIGNAL(signalChanged()), this, SLOT(changeDialog()));
    connect(this, SIGNAL(signalSaveChanges()), secondTab, SLOT(acceptCurrentChanges()));
}

optionsDialog::~optionsDialog()
{
    delete ui;
}


void optionsDialog::on_applyPushButton_clicked()
{
    emit signalSaveChanges();
    ui->applyPushButton->setEnabled(false);
    this->update();
}


void optionsDialog::changeDialog()
{
    ui->applyPushButton->setEnabled(true);
}
