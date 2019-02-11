#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <memory>
#include <QObject>

class startDialog;
class MainWidget;


class MainController: public QObject
{
    Q_OBJECT
public:
    MainController(QObject* parent = nullptr);
    ~MainController();

    void createStartDialog();

private slots:
    void slotCreateMainWidget(QString pixmapPath);
    void slotEndProgramm();
    void slotEndGame();
    void slotSaveScore(int score);
private:

    MainController(const MainController&) = delete;
    MainController& operator=(const MainController&) = delete;

    std::unique_ptr<startDialog> startD;
    std::unique_ptr<MainWidget> widget;
};

#endif // MAINCONTROLLER_H
