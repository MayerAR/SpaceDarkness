#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>

namespace Ui {
class startDialog;
}

//отвечает за начальный экран и запуск всех окон, за исключением игры -- за игру отвечает MainController
class startDialog : public QDialog
{
    Q_OBJECT

public:
    explicit startDialog(QWidget *parent = nullptr);
    ~startDialog() override;

signals:
    void signalStartGame(QString);
    void signalEndProgramm();

protected:
    void closeEvent(QCloseEvent *) override;
private slots:
    void on_startPushButton_clicked();
    void on_optionsPushButton_clicked();
    void on_keysPushButton_clicked();
    void on_leaderPushButton_clicked();
    void on_aboutPushButton_clicked();
    void on_aboutQtPushButton_clicked();

    void slotCreateStage(QString pixmapPath);
private:
    Ui::startDialog *ui;

};

#endif // STARTDIALOG_H
