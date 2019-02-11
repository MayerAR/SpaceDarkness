#ifndef GAMEOPTIONSTAB_H
#define GAMEOPTIONSTAB_H

#include <QWidget>
#include <QSettings>
#include <QTranslator>
#include <memory>

struct xmlElem;

namespace Ui {
class GameOptionsTab;
class ObjectOptionsTab;
}

class GameOptionsTab : public QWidget
{
    Q_OBJECT

public:
    explicit GameOptionsTab(QWidget *parent = nullptr);
    ~GameOptionsTab();
signals:
    void signalChanged();
public slots:
    void acceptCurrentChanges();
    void slotChanged();
private:

    Ui::GameOptionsTab *ui;
};

class ObjectOptionsTab: public QWidget
{
    Q_OBJECT
public:
    explicit ObjectOptionsTab(QWidget* parent = nullptr);
    ~ObjectOptionsTab();
signals:
    void signalChanged();
    void signalGoodName();
public slots:
    void checkNameOfSettings(QString name);
    void slotChanged(QString currentName);
    void acceptCurrentChanges();
    void on_deletePushButton_clicked();
    void on_createPushButton_clicked();
    void on_changePushButton_clicked();
private:
    bool ifInAllParam(const QString &name) const;

    std::shared_ptr<xmlElem> allParam;
    Ui::ObjectOptionsTab* ui;
};

#endif // GAMEOPTIONSTAB_H
