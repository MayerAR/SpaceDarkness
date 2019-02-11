#include "createnewobjectsparamdialog.h"
#include "ui_createnewobjectsparamdialog.h"
#include <QRegExpValidator>
#include <QMessageBox>


CreateNewObjectsParamDialog::CreateNewObjectsParamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNewObjectsParamDialog)
{
    ui->setupUi(this);
    QRegExp reg("[a-zA-Z]*");
    ui->nameSettingsLineEdit->setValidator(new QRegExpValidator(reg,this));
    textEdited("");
}

CreateNewObjectsParamDialog::~CreateNewObjectsParamDialog()
{
    delete ui;
}

void CreateNewObjectsParamDialog::goodNameOfSettings()
{
    this->close();
}

void CreateNewObjectsParamDialog::textEdited(QString curText)
{
    if(curText.size() > 0)
        ui->createPushButton->setEnabled(true);
    else
        ui->createPushButton->setEnabled(false);
}

void CreateNewObjectsParamDialog::on_createPushButton_clicked()
{
    emit newNameOfSettings(ui->nameSettingsLineEdit->text());
}

