#include "loadStage.h"
#include "readXmlFiles/readfile.h"
#include "gameWidget/objects/differentObjects/pixmapParametres/pixmapitem.h"
#include "gameWidget/objects/allobjects.h"
#include "gameWidget/strategy/InterfaceStrategy.h"
#include "gameWidget/animationItem/animationItem.h"
#include "gameWidget/objects/differentObjects/WeaponParam/weaponparam.h"
#include "gameWidget/objects/changeobjectdata.h"
#include "gameWidget/objects/differentObjects/AllObjectParam/allobjectparam.h"
#include <QDebug>
#include <QPen>
#include <cmath>

createObject* createObject::p_single = nullptr;

LoadWave::LoadWave(const std::shared_ptr<xmlElem>& waves_): waves(waves_)
{
    numbWave = 0;
    limitWave = waves->nextLevel.value("numbOfWaves")->value.toInt();

}

LoadWave::~LoadWave()
{

}

//call this from controller when one wave is over and we need to create next wave of current stage
//or when we create first wave of stage
QList<std::shared_ptr<GameObject>> LoadWave::nextWave()
{

    QChar symbOfWave = 'a' + numbWave; //choose wave a, b, c, d, ...
    QString nameOfWave(symbOfWave);

    auto thisWave = waves->nextLevel.value(nameOfWave); //current Wave
    QList<std::shared_ptr<GameObject>> objectsOfWave;
    auto create = createObject::getSingle();

    for(auto iter = thisWave->nextLevel.begin(), end = thisWave->nextLevel.end(); iter != end; iter++)//go throw the QMap of current wave
        objectsOfWave.push_back(create->createItem(iter.key(), iter.value())); //create all elements of current wave of current stage


    numbWave++;
    return objectsOfWave;
}

//check if we had next stage
bool LoadWave::ifNextWave() const
{
    QChar symbOfWave = 'a' + numbWave;
    QString nameOfWave(symbOfWave);

    if(numbWave < limitWave && findInXmlMap(waves, nameOfWave)) // if numbWave is not too big and if we find in xml current wave and ready to load this
        return true;                                         //we can do mistake in limitWave and in xml file
    return false;
}

void LoadWave::restartWaves()
{
    numbWave = 0;
}

//static class createObject
createObject::createObject()
{
    curParam = readXmlFile("files/objectParam/allObjectParam.xml")->nextLevel.value("param");
    curParam = curParam->nextLevel.value("allParam")->nextLevel.value(curParam->nextLevel.value("currentParam")->value);


    const QString shipModelPath = curParam->nextLevel.value("gameObjectParam")->value; //file that save all types of enemy objects
    const QString bulletModelPath = curParam->nextLevel.value("bulletParam")->value;//file that save all types of bullet
    const QString animationPath = ":/param/animation.xml"; //file that save animation
    const QString strategyPath = ":/param/strategy.xml"; //file that save animation


    auto ptr = readXmlFile(bulletModelPath);
    bulletModel = ptr->nextLevel.value("objects");


    ptr = readXmlFile(shipModelPath);
    shipModel = ptr->nextLevel.value("objects");

    ptr = readXmlFile(animationPath);
    animation = ptr->nextLevel.value("animation");

    ptr = readXmlFile(strategyPath);
    strategy = ptr->nextLevel.value("strategy");
}

createObject::~createObject()
{

    //delete old object
    bulletModel.reset(); //visible delete shared_ptr
    shipModel.reset();
    animation.reset();
}

void createObject::deleteSingle()
{
    delete p_single;
    p_single = nullptr; //because p_single is static
}

void createObject::setScale(const qreal &scale_)
{
    scale = scale_;
}

std::shared_ptr<GameObject> createObject::createItem(const QString &name, std::shared_ptr<xmlElem> param) const
{
    if(name == "Rock")
        return createRock(name, param);

    if(!findInXmlMap(shipModel, name))
        exit(40);
    auto thisShip = shipModel->nextLevel.value(name);
    auto typeOfItem = static_cast<objectID>(
                thisShip->attributes.value("id").toInt());
    auto typeOfBullet = static_cast<objectID>(
                thisShip->nextLevel.value("typeBullet")->value.toInt());
    qreal width = thisShip->nextLevel.value("width")->value.toInt() * scale;
    qreal height = thisShip->nextLevel.value("height")->value.toInt() * scale;
    auto pPath = thisShip->nextLevel.value("pixmapPath")->value;


    auto typeOfStrategy = static_cast<strategyID>(
                param->nextLevel.value("typeStrategy")->value.toInt());
    if(typeOfStrategy  < strategyID::first || typeOfStrategy  > strategyID::last)
        typeOfStrategy  = strategyID::BaseStrategy;


    std::shared_ptr<GameObject> newShip(new GameObject(pPath, QSize(width, height), typeOfItem, typeOfStrategy, typeOfBullet));


    auto thisStrategy = strategy->nextLevel.value(nameOfStrategy(typeOfStrategy)); //go to current strategy

//    if(typeOfStrategy  == strategyID::FirstShipStrategy)
//    {
//        auto firstStrategy = static_cast<FirstShipStrategy*>(newShip->curStrategy);
//        int minD = thisStrategy->nextLevel.value("minDist")->value.toInt();
//        int maxD = thisStrategy->nextLevel.value("maxDist")->value.toInt();
//        firstStrategy->setDistance(scale * randInt(minD, maxD));
//    }

    newShip->currentStrategy()->setBumbReaction(
                thisStrategy->nextLevel.value("bumbReaction")->value.toInt() // read current value
                == 1 //if it is 1 we send true ( 1 == 1) if other number we send false ( not 1 == 1 --> false)
                );



    newShip->setPosition(QPointF(
                         param->nextLevel.value("x_pos")->value.toDouble(),
                         param->nextLevel.value("y_pos")->value.toDouble()
                         ));


    newShip->setBoundingRect(QSize(
                                 scale * thisShip->nextLevel.value("boundingWidth")->value.toDouble(),
                                 scale * thisShip->nextLevel.value("boundingHeight")->value.toDouble()
                                 ));



    return newShip;
}

std::shared_ptr<GameObject> createObject::createMainItem() const
{
    const QString mainPath = ":/param/mainItem.xml"; //file that save weapon's type
    auto file = readXmlFile(mainPath);
    file = file->nextLevel.value("mainShip");
    auto typeOfStrategy = static_cast<strategyID>(
            file->nextLevel.value("typeStrategy")->value.toInt());
    auto typeOfPixmap = file->nextLevel.value("type")->attributes.value("typeOfPixmap");
    auto pPath = file->nextLevel.value("pixmapPath")->nextLevel.value(typeOfPixmap)->value;

    auto thisShip = shipModel->nextLevel.value( file->nextLevel.value("type")->value);

    auto typeOfItem = static_cast<objectID>(
                thisShip->attributes.value("id").toInt());
    auto typeOfBullet = static_cast<objectID>(
                thisShip->nextLevel.value("typeBullet")->value.toInt());
    qreal width = thisShip->nextLevel.value("width")->value.toInt() * scale;
    qreal height = thisShip->nextLevel.value("height")->value.toInt() * scale;

    std::shared_ptr<GameObject> item(new GameObject(pPath, QSize(width, height), typeOfItem, typeOfStrategy, typeOfBullet));


    item->setBoundingRect(QSize(
                                 scale * thisShip->nextLevel.value("boundingWidth")->value.toDouble(),
                                 scale * thisShip->nextLevel.value("boundingHeight")->value.toDouble()
                                 ));


    return item;
}


std::shared_ptr<GameObject> createObject::createRock(const QString &name, std::shared_ptr<xmlElem> param) const
{

    if(!findInXmlMap(shipModel, name))
        exit(40);
    auto thisRock = shipModel->nextLevel.value(name);

    auto typeOfItem = static_cast<objectID>(
                thisRock->attributes.value("id").toInt());
    auto typeOfStrategy = static_cast<strategyID>(param->nextLevel.value("typeStrategy")->value.toInt());
    auto typeOfBullet = objectID::none;
    qreal width = scale * thisRock->nextLevel.value("width")->value.toInt() * randInt(1, 3)/2;

    QString pixmapPath = thisRock->nextLevel.value("pixmapPath")->value;


    std::shared_ptr<GameObject> newRock(new GameObject(pixmapPath, QSize(width, width), typeOfItem, typeOfStrategy, typeOfBullet));

    auto thisStrategy = strategy->nextLevel.value(nameOfStrategy(typeOfStrategy)); //go to current strategy
    newRock->currentStrategy()->setBumbReaction(
                thisStrategy->nextLevel.value("bumbReaction")->value.toInt() // read current value
                == 1 //if it is 1 we send true ( 1 == 1) if other number we send false ( not 1 == 1 --> false)
                );
    newRock->setBoundingRect(QSize(width, width));
    newRock->setPosition(QPointF(param->nextLevel.value("x_pos")->value.toDouble(), param->nextLevel.value("y_pos")->value.toDouble()));

    std::shared_ptr<ChangeObjectData> changeData(new ChangeObjectData);

    changeData->setDeltaVelocity(Vector2D(
                                     scale * param->nextLevel.value
                                     ("velocity")->attributes.value("x").toDouble(),
                                     scale * param->nextLevel.value
                                     ("velocity")->attributes.value("y").toDouble()
                                     ));
    int limitZVelocity = static_cast<int>(AllObjectParam::getSingle()->currentParam(typeOfItem)->limitRotation);
    changeData->setDeltaZVelocity(randInt(0, limitZVelocity<<1) - limitZVelocity);

    newRock->update(changeData);

    return newRock;
}


std::shared_ptr<BulletObject> createObject::createBullet(const std::shared_ptr<GameObject> ship) const
{
    const QString name = nameOfObject(ship->idBullet()); //we find current id in xml file
    if(!findInXmlMap(bulletModel, name))
        exit(40);

    auto thisBullet = bulletModel->nextLevel.value(name);

    auto typeOfBullet = ship->idBullet();
    auto typeOfStrategy = static_cast<strategyID>(thisBullet->nextLevel.value("typeStrategy")->value.toInt());

    //set pixmap
    qreal width = scale * thisBullet->nextLevel.value("width")->value.toInt();
    qreal height = scale * thisBullet->nextLevel.value("height")->value.toInt();
    QString pixmapPath = thisBullet->nextLevel.value("pixmapPath")->value;

    std::shared_ptr<BulletObject> newBullet(new BulletObject(pixmapPath, QSize(width, height), typeOfBullet, typeOfStrategy));

    newBullet->setDamage(randInt(
                             thisBullet->nextLevel.value("damageMin")->value.toInt(),
                             thisBullet->nextLevel.value("damageMax")->value.toInt()
                             ));

    //set bounding
    newBullet->setBoundingRect(QSize(
                                   scale * thisBullet->nextLevel.value("boundingWidth")->value.toDouble(),
                                   scale * thisBullet->nextLevel.value("boundingHeight")->value.toDouble()));


    //calculate position of current bullet
    qreal x = 0;
    qreal y = -(5 + ship->pixmapItem()->pixmap().rect().height()); //because OY look down
    QPointF curPoint = ship->pixmapItem()->mapToScene(x, y);
    //set position
    newBullet->setPosition(curPoint);

    std::shared_ptr<ChangeObjectData> changeData(new ChangeObjectData());

    changeData->setAngleRad(ship->rotationAngleRad() - pi/2);

    qreal limitVelocity = AllObjectParam::getSingle()->currentParam(typeOfBullet)->limitVelocity;

    changeData->setDeltaVelocity(Vector2D(limitVelocity * sin(ship->rotationAngleRad()),
                                          -limitVelocity * cos(ship->rotationAngleRad())));

//    changeData->setAngleRad(0);//ship->rotationAngleRad() - pi/2);

    newBullet->update(changeData);




    return newBullet;
}

EndingAnimation *createObject::createAnimationExplosionBullet(objectID id, QGraphicsItem *parent) const
{
    const QString name = nameOfObject(id);

    if(!findInXmlMap(animation->nextLevel.value("explosion"), name))
    {
        qDebug()<<"Can't find! createAnimationExplosionBullet(...)";
        return nullptr;
    }
    auto thisAnimation = animation->nextLevel.value
            ("explosion")->nextLevel.value(name);

    EndingAnimation* newAnimation = new EndingAnimation(parent);

    int typeOfExplosion = randInt(0, thisAnimation->attributes.value("n").toInt());

    QChar symbOfKey = 'a' + typeOfExplosion;
    QString nameOfKey(symbOfKey);
    thisAnimation = thisAnimation->nextLevel.value(nameOfKey);

    newAnimation->mFrames = thisAnimation->nextLevel.value("frames")->value.toInt();
    newAnimation->mWidth = thisAnimation->nextLevel.value("fWidth")->value.toInt();
    newAnimation->mHeight = thisAnimation->nextLevel.value("fHeight")->value.toInt();
    newAnimation->mOffset = thisAnimation->nextLevel.value("offset")->value.toInt();
    newAnimation->mBorder = thisAnimation->nextLevel.value("border")->value.toInt();
    newAnimation->mTimeOut = thisAnimation->nextLevel.value("timeOut")->value.toInt();

    qreal width = scale * thisAnimation->nextLevel.value("width")->value.toDouble();
    qreal height = scale * thisAnimation->nextLevel.value("height")->value.toDouble();
    newAnimation->setBoundingRect(width, height);

    newAnimation->mPixmap = new QPixmap(thisAnimation->nextLevel.value("filePath")->value);
    return newAnimation;
}

QMap<objectID, std::shared_ptr<WeaponParam>>* createObject::createAllWeaponParam() const
{
    auto result = new QMap<objectID, std::shared_ptr<WeaponParam>>;

    const QString weaponPath = curParam->nextLevel.value("weaponParam")->value; //file that save weapon's type

    auto ptr = readXmlFile(weaponPath);
    auto allWeapons = ptr->nextLevel.value("objects");
    for(auto& elem: allWeapons->nextLevel)
    {
        result->insert(idOfObject(elem->name),
                      paramOfWeapon(elem));
    }

    std::shared_ptr<WeaponParam> empty(new WeaponParam);
    result->insert(objectID::none, empty);

    return result;
}

QMap<objectID, std::shared_ptr<ObjectParam>> *createObject::createAllObjectParam() const
{
    auto result = new QMap<objectID, std::shared_ptr<ObjectParam>>;

    const QString objectPath = curParam->nextLevel.value("objectParam")->value; //file that save weapon's type

    auto ptr = readXmlFile(objectPath);
    auto allObjects = ptr->nextLevel.value("objects")->nextLevel.value("item");
    for(auto& elem: allObjects->nextLevel)
    {
        result->insert(idOfObject(elem->name),
                      paramOfItem(elem));
    }
    auto allBullets = ptr->nextLevel.value("objects")->nextLevel.value("bullet");

    for(auto& elem: allBullets->nextLevel)
    {
        result->insert(idOfObject(elem->name),
                      paramOfBullet(elem));
    }
    result->insert(objectID::Border,
                   paramOfBorder(
                       ptr->nextLevel.value("objects")->nextLevel.value("border")
                       ));


//    std::shared_ptr<ObjectParam> empty(new ObjectParam);
//    result->insert(objectID::none, empty);

    return result;
}

std::shared_ptr<WeaponParam> createObject::paramOfWeapon(std::shared_ptr<xmlElem> param) const
{
    WeaponParam* curParam = new WeaponParam;
    curParam->clip = param->nextLevel.value("clip")->value.toInt();
    curParam->reloadTicks = param->nextLevel.value("reloadTicks")->value.toInt();
    curParam->betweenShotsTicks = param->nextLevel.value("betweenShotsTicks")->value.toInt();
    curParam->recoil = param->nextLevel.value("recoil")->value.toInt();
    return std::shared_ptr<WeaponParam>(curParam);
}

std::shared_ptr<ObjectParam> createObject::paramOfItem(std::shared_ptr<xmlElem> param) const
{
    std::shared_ptr<ObjectParam> curParam(new ObjectParam());

    //    ObjectParam* curParam = new ObjectParam();
    curParam->acceleration = scale * param->nextLevel.value("acceleration")->value.toDouble();
    curParam->limitVelocity = scale * param->nextLevel.value("limitVelocity")->value.toDouble();
    curParam->limitRotation = param->nextLevel.value("limitRotation")->value.toDouble();
    curParam->rotateAcceleration = param->nextLevel.value("rotateAcceleration")->value.toInt();
    curParam->isStatic = param->nextLevel.value("isStatic")->value.toInt() == 1;

    curParam->elasticity = param->nextLevel.value("elasticity")->value.toDouble();
    curParam->mass = param->nextLevel.value("mass")->value.toDouble();
    curParam->inv_mass = 1 / curParam->mass;
    curParam->health = param->nextLevel.value("health")->value.toInt();


    return curParam;
}

std::shared_ptr<ObjectParam> createObject::paramOfBullet(std::shared_ptr<xmlElem> param) const
{
    std::shared_ptr<ObjectParam> curParam(new ObjectParam());
    curParam->acceleration = scale * param->nextLevel.value("acceleration")->value.toDouble();
    curParam->limitVelocity = scale * param->nextLevel.value("limitVelocity")->value.toDouble();
    curParam->isStatic = param->nextLevel.value("isStatic")->value.toInt() == 1;

    curParam->limitRotation = 0;
    curParam->rotateAcceleration = 0;
    curParam->elasticity = 0;
    curParam->mass = 0;
    curParam->inv_mass = 1;
    curParam->health = 1;

    return curParam;

}

std::shared_ptr<ObjectParam> createObject::paramOfBorder(std::shared_ptr<xmlElem> param) const
{
    std::shared_ptr<ObjectParam> curParam(new ObjectParam());
    curParam->acceleration = 0;
    curParam->limitVelocity = 0;
    curParam->limitRotation = 0;
    curParam->rotateAcceleration = 0;
    curParam->health = 0;

    curParam->mass = param->nextLevel.value("mass")->value.toInt();
    curParam->inv_mass = 1 / curParam->mass;
    curParam->isStatic = param->nextLevel.value("isStatic")->value.toInt() == 1;
    curParam->elasticity = param->nextLevel.value("elasticity")->value.toInt();
    return curParam;
}




//    ShipData data;
//    data.acceleration = thisShip->nextLevel.value
//                        ("acceleration")->value.toInt() * scale;
//    data.rotateAcceleration =  thisShip->nextLevel.value
//                                ("rotateAcceleration")->value.toInt();
//    data.limitVelocity = static_cast<float>(
//            thisShip->nextLevel.value
//            ("limitVelocity")->value.toInt() * scale);

//    data.limitRotation = thisShip->nextLevel.value
//            ("limitRotation")->value.toInt();
//    data.elasticity = thisShip->nextLevel.value
//            ("elasticity")->value.toDouble();

//    data.mass = thisShip->nextLevel.value("mass")->value.toDouble();
//    data.health = thisShip->nextLevel.value("health")->value.toInt();

