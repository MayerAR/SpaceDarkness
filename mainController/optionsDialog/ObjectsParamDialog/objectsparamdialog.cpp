#include "objectsparamdialog.h"
#include "ui_objectsparamdialog.h"
#include "readXmlFiles/readfile.h"
#include "WidgetTab/widgettab.h"
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QPushButton>
#include "mainController/optionsDialog/gamesettings.h"

ObjectsParamDialog::ObjectsParamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ObjectsParamDialog)
{
    ui->setupUi(this);

}

ObjectsParamDialog::~ObjectsParamDialog()
{
    delete ui;
}

bool ObjectsParamDialog::loadTabs(const QString &name)
{
    nameOfCurrentParam = name;
    auto currentParam = readXmlFile("files/objectParam/allObjectParam.xml")->
                                            nextLevel.value("param")->nextLevel.value("allParam");

    QString paramName;
    if(currentParam->nextLevel.find(name) == currentParam->nextLevel.end())
    {
        this->setWindowTitle(tr("create NewObjectsParam: ") + name);
        paramName = "standartParam";
    }
    else
    {
        this->setWindowTitle(tr("change ") + name);
        paramName = name;
    }

    currentParam = currentParam->nextLevel.value(paramName);

    while(true)
    {
        try
        {
            objectParam = readXmlFile(currentParam->nextLevel.value("objectParam")->value);
            gameObjectParam = readXmlFile(currentParam->nextLevel.value("gameObjectParam")->value);
            bulletParam = readXmlFile(currentParam->nextLevel.value("bulletParam")->value);
            weaponParam = readXmlFile(currentParam->nextLevel.value("weaponParam")->value);
            createParam();
            this->show();
            return true;
        }
        catch(const QString& fileNotReading)
        {
            std::unique_ptr<QMessageBox> msg(new QMessageBox(this));
            msg->setIcon(QMessageBox::Critical);
            msg->setWindowTitle(tr("ERROR!"));
            msg->setText(tr("Couldn't Read this Param:\n") + fileNotReading);
            msg->setInformativeText(tr("Change current file on standart param file,\nor delete this param?"));
            auto changeButton = msg->addButton(tr("Change"), QMessageBox::ActionRole);
            auto deleteButton = msg->addButton(tr("Delete"), QMessageBox::ActionRole);
            msg->exec();
            if(msg->clickedButton() == changeButton)
            {
                changeMistakeOnStandart(fileNotReading, paramName);
            }
            else if (msg->clickedButton() == deleteButton)
            {
                deleteParam(paramName);
                return false;
            }
        }
    }
}

void ObjectsParamDialog::slotChanged()
{
    ui->acceptPushButton->setEnabled(true);
}

void ObjectsParamDialog::on_acceptPushButton_clicked()
{
    if(allTabWidget.size() == 0)
        return;
    //подготавливаем данные для их изменения
    auto items = objectParam->nextLevel.value("objects")->nextLevel.value("item");
    auto bullet = objectParam->nextLevel.value("objects")->nextLevel.value("bullet");
    auto gameObject = gameObjectParam->nextLevel.value("objects");
    auto bulletObject = bulletParam->nextLevel.value("objects");
    auto weaponObject = weaponParam->nextLevel.value("objects");

    std::shared_ptr<xmlElem> objects;
    std::shared_ptr<xmlElem> fileWithPixmapPath;
    for(const auto& elem: allTabWidget)
    {
        if(elem->needLoadData())
        {
            //выгружаем данные из WidgetTab
            auto currentData = elem->prepareData();

            if(currentData->name == "item") // в зависимости от типа данных - записываем в разные места
            {
                objects = items;
                fileWithPixmapPath = gameObject;
            }
            else
            {
                objects = bullet;
                fileWithPixmapPath = bulletObject;
                weaponObject->nextLevel.value(currentData->nextLevel.value("weaponParam")->value)->nextLevel =
                        currentData->nextLevel.value("weaponParam")->nextLevel;
            }
            //структура файлов одна( меняются только сами файлы)
            objects->nextLevel.value(currentData->nextLevel.value("objectData")->value)->nextLevel =
                        currentData->nextLevel.value("objectData")->nextLevel;

            if(currentData->nextLevel.value("pixmapPath")->value != "")
            {
                fileWithPixmapPath->nextLevel.value(currentData->nextLevel.value("objectData")->value)->
                        nextLevel.value("pixmapPath")->value =
                            currentData->nextLevel.value("pixmapPath")->value;
            }
        }
    }

    createAllFiles();
    this->accept();
}

void ObjectsParamDialog::createAllFiles()
{
    QDir directory("files/objectParam");

    if(!directory.exists(nameOfCurrentParam))
        directory.mkdir(nameOfCurrentParam);

    QString objectFilePath("files/objectParam/" + nameOfCurrentParam + "/objectParam.xml");
    writeXmlFile::writeFile(objectFilePath, objectParam);

    QString itemFilePath("files/objectParam/" + nameOfCurrentParam + "/gameObjectParam.xml");
    writeXmlFile::writeFile(itemFilePath, gameObjectParam);

    QString bulletFilePath("files/objectParam/" + nameOfCurrentParam + "/bulletParam.xml");
    writeXmlFile::writeFile(bulletFilePath, bulletParam);

    QString weaponFilePath("files/objectParam/" + nameOfCurrentParam + "/weaponObject.xml");
    writeXmlFile::writeFile(weaponFilePath, weaponParam);
}

void ObjectsParamDialog::createParam()
{
    auto items = objectParam->nextLevel.value("objects")->nextLevel.value("item")->nextLevel;
    auto gameObjects = gameObjectParam->nextLevel.value("objects");
//            gameObjectParam->nextLevel("objects").
    for(const auto& elem: items)
    {

        AbstractWidgetTab* curWidget = new WidgetItemTab(elem,
                                                     gameObjects->nextLevel.value(elem->name),
                                                     this);

        connect(curWidget, SIGNAL(signalChanged()), this, SLOT(slotChanged()));
        ui->objectsParamTabWidget->addTab(curWidget, elem->name);

        allTabWidget.push_back(curWidget);
    }

    auto bullets = objectParam->nextLevel.value("objects")->nextLevel.value("bullet")->nextLevel;
    auto bulletObjects = bulletParam->nextLevel.value("objects");
    auto weaponObjects = weaponParam->nextLevel.value("objects");
    for(const auto& elem: bullets)
    {

        AbstractWidgetTab* curWidget = new WidgetBulletTab(elem,
                                                     bulletObjects->nextLevel.value(elem->name),
                                                     weaponObjects->nextLevel.value(elem->name),
                                                           this);

        connect(curWidget, SIGNAL(signalChanged()), this, SLOT(slotChanged()));
        ui->objectsParamTabWidget->addTab(curWidget, elem->name);

        allTabWidget.push_back(curWidget);
    }
}

void ObjectsParamDialog::changeMistakeOnStandart(const QString &filePath, const QString &paramName)
{
    //функция меняет имя файла, копирует данные из стандартного файла
    QDir currentDirectory("files/objectParam");
    if(!currentDirectory.exists(paramName))
        currentDirectory.mkdir(paramName);

    auto currentFile = readXmlFile("files/objectParam/allObjectParam.xml");
    auto currentParam = currentFile->nextLevel.value("param")->nextLevel.value("allParam")->nextLevel.value(paramName);
    QString newStandartFilePath;
    QString wrongTag;
    for(auto& fileElem: currentParam->nextLevel)
    {
        if(fileElem->value == filePath)
        {
            wrongTag = fileElem->name;
            newStandartFilePath = "files/objectParam/" + paramName + "/" + wrongTag + ".xml"; //создали новое имя файла
            fileElem->value = newStandartFilePath; //изменили путь к файлу в allObjectsParam
        }
    }
    QFile mistakeFile(filePath);
    if(mistakeFile.exists())
        mistakeFile.remove();  // если существует старая версия --> удаляем

    writeXmlFile::writeFile("files/objectParam/allObjectParam.xml", currentFile); //перезаписываем allObjectsParam

    auto currentStandartaParamPath = currentFile->nextLevel.value("param")->
            nextLevel.value("allParam")->nextLevel.value("standartParam")->
            nextLevel.value(wrongTag)->value;

    writeXmlFile::writeFile(newStandartFilePath, readXmlFile(currentStandartaParamPath));// в новом месте записываем стандартные значения

}

void ObjectsParamDialog::deleteParam(const QString& paramName)
{
    //удалает все упоминания paramName
    auto currentFile = readXmlFile("files/objectParam/allObjectParam.xml");
    auto allParam = currentFile->nextLevel.value("param")->nextLevel.value("allParam");
    if(allParam->nextLevel.find(paramName) != allParam->nextLevel.end())
    {
        allParam->nextLevel.remove(paramName); //удаляем из allObjetsParam
    }
    currentFile->nextLevel.value("param")->nextLevel.value("currentParam")->value = "standartParam";

    writeXmlFile::writeFile("files/objectParam/allObjectParam.xml", currentFile); //перезаписываем allObjectsParam

    deleteDirectory("files/objectParam/" + paramName);
}
