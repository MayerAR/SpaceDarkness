#include "gameoptionstab.h"
#include "ui_gameoptionstab.h"
#include "ui_objectsoptionstab.h"
#include "gamesettings.h"
#include "readXmlFiles/readfile.h"
#include "mainController/optionsDialog/ObjectsParamDialog/objectsparamdialog.h"
#include "mainController/optionsDialog/CreateNewObjectsParamDialog/createnewobjectsparamdialog.h"
#include <QMessageBox>
#include <QDir>
#include <QDebug>

GameOptionsTab::GameOptionsTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameOptionsTab)
{
    ui->setupUi(this);
    ui->languageComboBox->addItem("English", "en_EN");
    ui->languageComboBox->addItem("Russian", "ru_RU");
    ui->languageComboBox->setCurrentText(GameSettings::getSingle().curLanguage());

    ui->musicHorizontalSlider->setValue(GameSettings::getSingle().curVolume());
    ui->musicHorizontalSlider->valueChanged(ui->musicHorizontalSlider->value());
}

GameOptionsTab::~GameOptionsTab()
{
    delete ui;
}

void GameOptionsTab::acceptCurrentChanges()
{
    GameSettings::getSingle().setVolume(ui->musicHorizontalSlider->value());
    if(GameSettings::getSingle().curLanguage() != ui->languageComboBox->currentText())
    {
        GameSettings::getSingle().setLanguage(ui->languageComboBox->currentText(),
                                              ui->languageComboBox->currentData().toString());
        QMessageBox msg;
        msg.setIcon(QMessageBox::Icon::Information);
        msg.setWindowTitle(tr("Information"));
        msg.setText(tr("Change Language will take effect after restarting this program"));
        msg.setStandardButtons(QMessageBox::StandardButton::Ok);
        msg.exec();
//        ui->retranslateUi(this);
    }
}

void GameOptionsTab::slotChanged()
{
    emit signalChanged();
}

ObjectOptionsTab::ObjectOptionsTab(QWidget *parent):
    QWidget(parent),
    ui(new Ui::ObjectOptionsTab)

{
    ui->setupUi(this);
    allParam = readXmlFile("files/objectParam/allObjectParam.xml");

    for(const auto& elem: allParam->nextLevel.value("param")->nextLevel.value("allParam")->nextLevel)
        ui->changeObjectComboBox->addItem(elem->name);
    ui->changeObjectComboBox->setCurrentText(
                allParam->nextLevel.value("param")->nextLevel.value("currentParam")->value);

}

ObjectOptionsTab::~ObjectOptionsTab()
{
    delete ui;
}

void ObjectOptionsTab::checkNameOfSettings(QString name)
{

    //найти в allParam текущее имя - если есть slotBadName, если нет -- slotGoodName
    //затем создаем ObjectsParamDialog
    if(ifInAllParam(name))
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Information);
        msg.setWindowTitle(tr("Wrong name Of Settings"));
        msg.setText(tr("This name of paramteres are allready use.\nPlease change name of parametres."));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
    }

    emit signalGoodName();

    ObjectsParamDialog dialog(this);
    dialog.loadTabs(name);
    int res = dialog.exec();
    if(res == 1)
    {
        ui->changeObjectComboBox->addItem(name);
        ui->changeObjectComboBox->setCurrentText(name);

        //save changes in allParam file
        QString directoryPath = "files/objectParam/" + name;
        std::shared_ptr<xmlElem> objectParam(new xmlElem);
        objectParam->name = "objectParam";
        objectParam->value = directoryPath + "/objectParam.xml";

        std::shared_ptr<xmlElem> bulletParam(new xmlElem);
        bulletParam->name = "bulletParam";
        bulletParam->value = directoryPath + "/bulletParam.xml";

        std::shared_ptr<xmlElem> gameObjectParam(new xmlElem);
        gameObjectParam->name = "gameObjectParam";
        gameObjectParam->value = directoryPath + "/gameObjectParam.xml";

        std::shared_ptr<xmlElem> weaponParam(new xmlElem);
        weaponParam->name = "weaponParam";
        weaponParam->value = directoryPath + "/weaponObject.xml";



        std::shared_ptr<xmlElem> currentParam(new xmlElem);
        currentParam->name = name;
        currentParam->nextLevel.insert(objectParam->name, objectParam);
        currentParam->nextLevel.insert(bulletParam->name, bulletParam);
        currentParam->nextLevel.insert(gameObjectParam->name, gameObjectParam);
        currentParam->nextLevel.insert(weaponParam->name, weaponParam);

        allParam->nextLevel.value("param")->nextLevel.value("allParam")
                ->nextLevel.insert(currentParam->name, currentParam);
        writeXmlFile::writeFile("files/objectParam/allObjectParam.xml", allParam);
    }
    return;
}

void ObjectOptionsTab::slotChanged(QString currentName)
{
    if(currentName == "standartParam")
    {
        ui->deletePushButton->setEnabled(false);
        ui->changePushButton->setEnabled(false);
    }
    else
    {
        ui->deletePushButton->setEnabled(true);
        ui->changePushButton->setEnabled(true);
    }
    emit signalChanged();
}

void ObjectOptionsTab::acceptCurrentChanges()
{
    allParam->nextLevel.value("param")->nextLevel.value("currentParam")->value =
            ui->changeObjectComboBox->currentText();

    QStringList paramWeMustDelete;
    for(const auto& elem: allParam->nextLevel.value("param")->
                                    nextLevel.value("allParam")->nextLevel)
    {
        if(ui->changeObjectComboBox->findText(elem->name) == -1)
        {
            paramWeMustDelete.push_back(elem->name);
        }
    }
    for(const auto& name: paramWeMustDelete)
    {
        allParam->nextLevel.value("param")->nextLevel.value("allParam")
                ->nextLevel.remove(name);
        deleteDirectory("files/objectParam/" + name);
    }

    writeXmlFile::writeFile("files/objectParam/allObjectParam.xml", allParam);
}

void ObjectOptionsTab::on_deletePushButton_clicked()
{

    ui->changeObjectComboBox->removeItem(ui->changeObjectComboBox->currentIndex());
    emit signalChanged();
}

void ObjectOptionsTab::on_createPushButton_clicked()
{
    CreateNewObjectsParamDialog* dialog = new CreateNewObjectsParamDialog(this);
    connect(dialog, SIGNAL(newNameOfSettings(QString)), this, SLOT(checkNameOfSettings(QString)));
    connect(this, SIGNAL(signalGoodName()), dialog, SLOT(goodNameOfSettings()));
    dialog->exec();
}

void ObjectOptionsTab::on_changePushButton_clicked()
{
    ObjectsParamDialog dialog(this);
    dialog.loadTabs(ui->changeObjectComboBox->currentText());
    dialog.exec();
}

bool ObjectOptionsTab::ifInAllParam(const QString& name) const
{
    auto param = allParam->nextLevel.value("param")->nextLevel.value("allParam");
    if(!param->nextLevel.contains(name))
        return false;

    return true;
}
