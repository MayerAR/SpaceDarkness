#include "gamesettings.h"
#include <QApplication>
#include <QDir>


GameSettings::GameSettings():
    settings("MayerARCompany", "SpaceDarkness")
{
    translator.load(settings.value("options/language/prefix").toString(), ":/lang/");
    qApp->installTranslator(&translator);
}

GameSettings& GameSettings::getSingle()
{
    static GameSettings curSettings;
    return curSettings;
}

int GameSettings::curVolume() const
{
    return settings.value("options/volume").toInt();
}

QString GameSettings::curLanguage() const
{
    return settings.value("options/language").toString();
}

bool GameSettings::setVolume(int value)
{
    settings.setValue("options/volume", value);
    return true;
}

bool GameSettings::setLanguage(QString Text, QString Data)
{
    settings.setValue("options/language", Text);
    settings.setValue("options/language/prefix", Data);
    return true;
}

bool GameSettings::loadLanguage()
{

    bool ifGood = translator.load(settings.value("options/language/prefix").toString(), ":/lang/");
    qApp->installTranslator(&translator);
    return ifGood;
}




void deleteDirectory(const QString &dirPath)
{
    QDir dir(dirPath);
    if(dir.exists())
    {
        auto fileInfo = dir.entryInfoList();
        for(const auto& info: fileInfo)
            dir.remove(info.fileName());

        QString name = dir.dirName();
        dir.cdUp(); //поднимаемя по каталогу
        dir.rmdir(name);// удаляем  папку
    }
}
