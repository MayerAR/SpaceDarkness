#-------------------------------------------------
#
# Project created by QtCreator 2018-12-03T18:51:41
#
#-------------------------------------------------

TRANSLATIONS += \
    Translations/spacedarkness_ru.ts


QT += core gui
QT += xml
CONFIG += c++14


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpaceDarkness
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        gameWidget/widget.cpp \
    gameWidget/constants.cpp \
    gameWidget/contactItems/contact.cpp \
    gameWidget/controller/controller.cpp \
    gameWidget/controller/makeMove.cpp \
    gameWidget/loadStage/loadStage.cpp \
    readXmlFiles/readfile.cpp \
    gameWidget/objects/object.cpp \
    gameWidget/objects/differentObjects/pixmapParametres/pixmapitem.cpp \
    gameWidget/objects/differentObjects/WeaponParam/weaponparam.cpp \
    gameWidget/objects/differentObjects/bulletobject.cpp \
    gameWidget/objects/differentObjects/gameobject.cpp \
    gameWidget/strategy/objectStrategy/basestrategy.cpp \
    gameWidget/systemItem/singlecursor.cpp \
    gameWidget/strategy/InterfaceStrategy.cpp \
    gameWidget/objects/differentObjects/pixmapParametres/edge.cpp \
    gameWidget/objects/differentObjects/AllObjectParam/allobjectparam.cpp \
    gameWidget/mainItem/mainItem.cpp \
    gameWidget/customScene/customscene.cpp \
    gameWidget/customScene/customview.cpp \
    gameWidget/objects/changeobjectdata.cpp \
    gameWidget/strategy/objectStrategy/firstshipstrategy.cpp \
    gameWidget/strategy/objectStrategy/avaidClasses/line2d.cpp \
    mainController/chooseStageWidget/choosestagewidget.cpp \
    mainController/mainController.cpp \
    mainController/startDialog/startdialog.cpp \
    mainController/optionsDialog/optionsdialog.cpp \
    mainController/optionsDialog/CreateNewObjectsParamDialog/createnewobjectsparamdialog.cpp \
    mainController/optionsDialog/ObjectsParamDialog/objectsparamdialog.cpp \
    mainController/optionsDialog/ObjectsParamDialog/WidgetTab/widgettab.cpp \
    mainController/optionsDialog/gameoptionstab.cpp \
    mainController/optionsDialog/gamesettings.cpp \
    gameWidget/animationItem/animationItem.cpp \
    gameWidget/objects/differentObjects/borderobject.cpp

HEADERS += \
    gameWidget/widget.h \
    gameWidget/constants.h \
    gameWidget/contactItems/contact.h \
    gameWidget/controller/controller.h \
    gameWidget/controller/makeMove.h \
    gameWidget/loadStage/loadStage.h \
    readXmlFiles/readfile.h \
    gameWidget/objects/changeobjectdata.h \
    gameWidget/objects/object.h \
    gameWidget/objects/differentObjects/WeaponParam/weaponparam.h \
    gameWidget/objects/differentObjects/gameobject.h \
    gameWidget/strategy/objectStrategy/basestrategy.h \
    gameWidget/strategy/InterfaceStrategy.h \
    gameWidget/systemItem/singlecursor.h \
    gameWidget/objects/differentObjects/pixmapParametres/edge.h \
    gameWidget/objects/differentObjects/AllObjectParam/allobjectparam.h \
    gameWidget/objects/allobjects.h \
    gameWidget/mainItem/mainItem.h \
    gameWidget/customScene/customscene.h \
    gameWidget/customScene/customview.h \
    gameWidget/strategy/objectStrategy/firstshipstrategy.h \
    gameWidget/strategy/objectStrategy/avaidClasses/line2d.h \
    mainController/chooseStageWidget/choosestagewidget.h \
    mainController/mainController.h \
    gameWidget/objects/differentObjects/pixmapParametres/pixmapitem.h \
    gameWidget/objects/differentObjects/bulletobject.h \
    gameWidget/objects/allobjects.h \
    mainController/startDialog/startdialog.h \
    mainController/optionsDialog/optionsdialog.h \
    mainController/optionsDialog/CreateNewObjectsParamDialog/createnewobjectsparamdialog.h \
    mainController/optionsDialog/ObjectsParamDialog/objectsparamdialog.h \
    mainController/optionsDialog/ObjectsParamDialog/WidgetTab/widgettab.h \
    mainController/optionsDialog/gameoptionstab.h \
    mainController/optionsDialog/gamesettings.h \
    gameWidget/animationItem/animationItem.h \
    gameWidget/objects/differentObjects/borderobject.h

FORMS += \
    mainController/chooseStageWidget/choosestagewidget.ui \
    mainController/startDialog/startdialog.ui \
    mainController/optionsDialog/optionsdialog.ui \
    mainController/optionsDialog/CreateNewObjectsParamDialog/createnewobjectsparamdialog.ui \
    mainController/optionsDialog/ObjectsParamDialog/objectsparamdialog.ui \
    mainController/optionsDialog/ObjectsParamDialog/WidgetTab/widgetitemtab.ui \
    mainController/optionsDialog/ObjectsParamDialog/WidgetTab/widgetbullettab.ui \
    mainController/optionsDialog/gameoptionstab.ui \
    mainController/optionsDialog/objectsoptionstab.ui

RESOURCES += \
    resources/images.qrc \
    resources/objetsParam.qrc \
    resources/stages.qrc \
    Translations/translations.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
