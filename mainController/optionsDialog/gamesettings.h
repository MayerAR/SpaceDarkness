#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <QSettings>
#include <QTranslator>


class GameSettings
{
public:
    static GameSettings& getSingle();
    int curVolume() const;
    QString curLanguage() const;
    bool setVolume(int value);
    bool setLanguage(QString Text, QString Data);
    bool loadLanguage();
private:
    GameSettings();
    GameSettings(const GameSettings&) = delete;
    GameSettings& operator=(const GameSettings&) = delete;

    QSettings settings;
    QTranslator translator;
};

void deleteDirectory(const QString& dirPath);

#endif // GAMESETTINGS_H
