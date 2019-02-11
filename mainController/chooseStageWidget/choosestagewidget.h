#ifndef CHOOSESTAGEWIDGET_H
#define CHOOSESTAGEWIDGET_H

#include <QDialog>

namespace Ui {
class ChooseStageWidget;
}


class ChooseStageWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseStageWidget(const QString& allStagesPath_, QWidget *parent = nullptr);
    ~ChooseStageWidget();

    void updateComboBox();

signals:
    void signalCreateNewStage(const QString& stagePath);
private slots:
    void on_okeyButton_clicked();

private:
    QString allStagesPath;
    Ui::ChooseStageWidget *ui;    

};

#endif // CHOOSESTAGEWIDGET_H
