#include "widgettab.h"
#include "ui_widgetitemtab.h"
#include "ui_widgetbullettab.h"


#include "readXmlFiles/readfile.h"
#include <QMessageBox>
#include <QPixmap>
#include <QFileDialog>

AbstractWidgetTab::AbstractWidgetTab(QWidget *parent):
    QWidget(parent),
    ifChanged(false)
{
}

AbstractWidgetTab::~AbstractWidgetTab()
{
}

std::shared_ptr<xmlElem> AbstractWidgetTab::prepareData() const
{
    return readData();
}

bool AbstractWidgetTab::needLoadData() const
{
    return ifChanged;
}

void AbstractWidgetTab::slotChanged()
{
    if(!ifChanged)
    {
        ifChanged = true;
        emit signalChanged();
    }
}




WidgetItemTab::WidgetItemTab(std::shared_ptr<xmlElem> data,
                             std::shared_ptr<xmlElem> gameObjectData,
                             QWidget *parent) :
    AbstractWidgetTab(parent),
    ui(new Ui::WidgetItemTab)
{

    ui->setupUi(this);
    this->setWindowTitle(data->name);

    ui->accelerationSpinBox->setValue(data->nextLevel.value("acceleration")->value.toInt());
    ui->limitVelocitySpinBox->setValue(data->nextLevel.value("limitVelocity")->value.toInt());
    ui->limitRotationSpinBox->setValue(data->nextLevel.value("limitRotation")->value.toDouble());
    ui->rotateAccelerationSpinBox->setValue(data->nextLevel.value("rotateAcceleration")->value.toDouble());
    ui->elasticitySpinBox->setValue(data->nextLevel.value("elasticity")->value.toDouble());
    ui->isStaticCheckBox->setCheckState(data->nextLevel.value("isStatic")->value.toInt() == 0 ?
                                        Qt::CheckState::Unchecked : Qt::CheckState::Checked);

    connect(ui->isStaticCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slotChangeIsStatic(int)));
    ui->isStaticCheckBox->stateChanged(ui->isStaticCheckBox->checkState());

    ui->healthSlider->setValue(data->nextLevel.value("health")->value.toInt());
    ui->healthSlider->valueChanged(ui->healthSlider->value());
    ui->massSlider->setValue(data->nextLevel.value("mass")->value.toInt());
    ui->massSlider->valueChanged(ui->massSlider->value());

    connect(ui->changePixmapPushButton, SIGNAL(clicked()), this, SLOT(slotChangePixmap()));
    ifChanged = false;

    if(!setNewPixmap(gameObjectData->nextLevel.value("pixmapPath")->value))
    {
        //если не удалось загрузить -- загружаем стандартную картинку
        auto standartPixmapFilePath = readXmlFile(":/param/gameObjects.xml")->
                nextLevel.value("objects")->
                nextLevel.value(gameObjectData->name)->
                nextLevel.value("pixmapPath")->value;
        setNewPixmap(standartPixmapFilePath);
        pixmapPath = standartPixmapFilePath;
        slotChanged();
    }


}


WidgetItemTab::~WidgetItemTab()
{
    delete ui;
}


void WidgetItemTab::slotChangeIsStatic(int newValue)
{
    ui->isStaticCheckBox->setText(newValue == 0 ? tr("No") : tr("Yes"));
}


void WidgetItemTab::slotChangePixmap()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                        tr("Open Image"),"", tr("Image Files (*.png *.jpg *.bmp)"));
    if(setNewPixmap(fileName))
        pixmapPath = fileName;
}

std::shared_ptr<xmlElem> WidgetItemTab::readData() const
{
    std::shared_ptr<xmlElem> objectData(new xmlElem());
    objectData->name = "objectData";
    objectData->value = this->windowTitle();

    std::shared_ptr<xmlElem> accelerationData(new xmlElem());
    accelerationData->name = "acceleration";
    accelerationData->value = QString::number(ui->accelerationSpinBox->value());
    objectData->nextLevel.insert(accelerationData->name, accelerationData);

    std::shared_ptr<xmlElem> limitVelocityData(new xmlElem());
    limitVelocityData->name = "limitVelocity";
    limitVelocityData->value = QString::number(ui->limitVelocitySpinBox->value());
    objectData->nextLevel.insert(limitVelocityData->name, limitVelocityData);

    std::shared_ptr<xmlElem> limitRotationData(new xmlElem());
    limitRotationData->name = "limitRotation";
    limitRotationData->value = QString::number(ui->limitRotationSpinBox->value());
    objectData->nextLevel.insert(limitRotationData->name, limitRotationData);

    std::shared_ptr<xmlElem> rotateAccelerationData(new xmlElem());
    rotateAccelerationData->name = "rotateAcceleration";
    rotateAccelerationData->value = QString::number(ui->rotateAccelerationSpinBox->value());
    objectData->nextLevel.insert(rotateAccelerationData->name, rotateAccelerationData);

    std::shared_ptr<xmlElem> isStaticData(new xmlElem());
    isStaticData->name = "isStatic";
    isStaticData->value = ui->isStaticCheckBox->checkState() == Qt::CheckState::Checked ? "1" : "0";
    objectData->nextLevel.insert(isStaticData->name, isStaticData);

    std::shared_ptr<xmlElem> elasticityData(new xmlElem());
    elasticityData->name = "elasticity";
    elasticityData->value = QString::number(ui->elasticitySpinBox->value());
    objectData->nextLevel.insert(elasticityData->name, elasticityData);

    std::shared_ptr<xmlElem> healthData(new xmlElem());
    healthData->name = "health";
    healthData->value = ui->healthValueLabel->text();
    objectData->nextLevel.insert(healthData->name, healthData);

    std::shared_ptr<xmlElem> massData(new xmlElem());
    massData->name = "mass";
    massData->value = ui->massValueLabel->text();
    objectData->nextLevel.insert(massData->name, massData);

    std::shared_ptr<xmlElem> gameObjectData(new xmlElem());
    gameObjectData->name = "pixmapPath";
    gameObjectData->value = pixmapPath;

    std::shared_ptr<xmlElem> result(new xmlElem());
    result->name = "item";
    result->nextLevel.insert(objectData->name, objectData);
    result->nextLevel.insert(gameObjectData->name, gameObjectData);

    //qweqwe

    return result;
}

bool WidgetItemTab::setNewPixmap(const QString &pixmapPath)
{
    QPixmap currentPixmap;
    if(!currentPixmap.load(pixmapPath))
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setText(tr("Couldn't open current pixmap:\n") + pixmapPath);
        msg.setStandardButtons(QMessageBox::StandardButton::Ok);
        msg.exec();
        return false;
    }
    auto size = currentPixmap.size();
    if(size.height() > size.width())
    {
        QTransform transform;
        transform.rotate(90);
        currentPixmap = currentPixmap.transformed(transform);
    }
    currentPixmap = currentPixmap.scaledToWidth(ui->pixmapLabel->width());
    ui->pixmapLabel->setPixmap(currentPixmap);
    return true;
}




WidgetBulletTab::WidgetBulletTab(std::shared_ptr<xmlElem> data,
                                 std::shared_ptr<xmlElem> bulletData,
                                 std::shared_ptr<xmlElem> weaponParam,
                                 QWidget *parent):
    AbstractWidgetTab(parent),
    ui(new Ui::WidgetBulletTab)
{
    ui->setupUi(this);
    this->setWindowTitle(data->name);

    ui->accelerationSpinBox->setValue(data->nextLevel.value("acceleration")->value.toInt());
    ui->limitVelocitySpinBox->setValue(data->nextLevel.value("limitVelocity")->value.toInt());
    ui->recoilSpinBox->setValue(weaponParam->nextLevel.value("recoil")->value.toInt());
    ui->clipSpinBox->setValue(weaponParam->nextLevel.value("clip")->value.toInt());
    ui->reloadTimeSpinBox->setValue(weaponParam->nextLevel.value("reloadTicks")->value.toInt());
    ui->betweenShotTimeSpinBox->setValue(weaponParam->nextLevel.value("betweenShotsTicks")->value.toInt());


    ui->isStaticCheckBox->setCheckState(data->nextLevel.value("isStatic")->value.toInt() == 0 ?
                                            Qt::CheckState::Unchecked : Qt::CheckState::Checked);

    connect(ui->isStaticCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slotChangeIsStatic(int)));
    ui->isStaticCheckBox->stateChanged(ui->isStaticCheckBox->checkState());

    if(!setNewPixmap(bulletData->nextLevel.value("pixmapPath")->value))
    {
        //если не удалось загрузить -- загружаем стандартную картинку
        auto standartPixmapFilePath = readXmlFile(":/param/bulletObjects.xml")->
                nextLevel.value("objects")->
                nextLevel.value(bulletData->name)->value;
        setNewPixmap(standartPixmapFilePath);
        slotChanged();
    }
    connect(ui->changePixmapPushButton, SIGNAL(clicked()), this, SLOT(slotChangePixmap()));


    ifChanged = false;
}

WidgetBulletTab::~WidgetBulletTab()
{
    delete ui;
}

void WidgetBulletTab::slotChangeIsStatic(int newValue)
{
    ui->isStaticCheckBox->setText(newValue == 0 ? tr("No") : tr("Yes"));
}

void WidgetBulletTab::slotChangePixmap()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                        tr("Open Image"),"", tr("Image Files (*.png *.jpg *.bmp)"));
    if(setNewPixmap(fileName))
        pixmapPath = fileName;
}

std::shared_ptr<xmlElem> WidgetBulletTab::readData() const
{
    std::shared_ptr<xmlElem> result(new xmlElem());
    result->name = "bullet";

    std::shared_ptr<xmlElem> gameObjectData(new xmlElem());
    gameObjectData->name = "pixmapPath";
    gameObjectData->value = pixmapPath;

    result->nextLevel.insert(gameObjectData->name, gameObjectData);

    auto objectData = readBulletData();
    result->nextLevel.insert(objectData->name, objectData);

    auto weaponData = readWeaponData();
    result->nextLevel.insert(weaponData->name, weaponData);

    return result;
}

std::shared_ptr<xmlElem> WidgetBulletTab::readBulletData() const
{
    std::shared_ptr<xmlElem> objectData(new xmlElem());
    objectData->name = "objectData";
    objectData->value = this->windowTitle();

    std::shared_ptr<xmlElem> accelerationData(new xmlElem());
    accelerationData->name = "acceleration";
    accelerationData->value = QString::number(ui->accelerationSpinBox->value());
    objectData->nextLevel.insert(accelerationData->name, accelerationData);

    std::shared_ptr<xmlElem> limitVelocityData(new xmlElem());
    limitVelocityData->name = "limitVelocity";
    limitVelocityData->value = QString::number(ui->limitVelocitySpinBox->value());
    objectData->nextLevel.insert(limitVelocityData->name, limitVelocityData);


    std::shared_ptr<xmlElem> isStaticData(new xmlElem());
    isStaticData->name = "isStatic";
    isStaticData->value = ui->isStaticCheckBox->checkState() == Qt::CheckState::Checked ? "1" : "0";
    objectData->nextLevel.insert(isStaticData->name, isStaticData);

    return objectData;
}

std::shared_ptr<xmlElem> WidgetBulletTab::readWeaponData() const
{
    std::shared_ptr<xmlElem> weaponData(new xmlElem());
    weaponData->name = "weaponParam";
    weaponData->value = this->windowTitle();

    std::shared_ptr<xmlElem> clipData(new xmlElem());
    clipData->name = "clip";
    clipData->value = QString::number(ui->clipSpinBox->value());
    weaponData->nextLevel.insert(clipData->name, clipData);

    std::shared_ptr<xmlElem> reloadData(new xmlElem);
    reloadData->name = "reloadTicks";
    reloadData->value = QString::number(ui->reloadTimeSpinBox->value());
    weaponData->nextLevel.insert(reloadData->name, reloadData);

    std::shared_ptr<xmlElem> recoilData(new xmlElem);
    recoilData->name = "recoil";
    recoilData->value = QString::number(ui->recoilSpinBox->value());
    weaponData->nextLevel.insert(recoilData->name, recoilData);


    std::shared_ptr<xmlElem> betweenShotData(new xmlElem);
    betweenShotData->name = "betweenShotsTicks";
    betweenShotData->value = QString::number(ui->betweenShotTimeSpinBox->value());
    weaponData->nextLevel.insert(betweenShotData->name, betweenShotData);

    return weaponData;
}

bool WidgetBulletTab::setNewPixmap(const QString &pixmapPath)
{
    QPixmap currentPixmap;
    if(!currentPixmap.load(pixmapPath))
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setText(tr("Couldn't open current pixmap:\n") + pixmapPath);
        msg.setStandardButtons(QMessageBox::StandardButton::Ok);
        msg.exec();
        return false;
    }
    auto size = currentPixmap.size();
    if(size.height() > size.width())
    {
        QTransform transform;
        transform.rotate(90);
        currentPixmap = currentPixmap.transformed(transform);
    }
    currentPixmap = currentPixmap.scaledToWidth(ui->pixmapLabel->width());
    ui->pixmapLabel->setPixmap(currentPixmap);
    return true;
}
