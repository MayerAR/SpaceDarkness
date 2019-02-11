#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <memory>

class Controller;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(const QString& stagePath, QWidget *parent = nullptr);
    ~MainWidget() override;



    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *) override;
signals:
    //signal that say that we must to return to the start dialog
    void signalEndOfGame();
    //signal that say that we must save score ant return to the start dialog
    void signalEndOfStage(int score);
public slots:
    void slotEndOfStage(int score);
    void slotDestroedMainItem();
    void slotChangeCursorPosition(const QPoint &newSceneMousePos);
private:
    void pauseWidget();

    std::unique_ptr<Controller> contr;

};

#endif // WIDGET_H
