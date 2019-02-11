#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <memory>
#include <QTranslator>
#include <QSettings>

struct xmlElem;


namespace Ui {
class optionsDialog;
}


class optionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit optionsDialog(QWidget *parent = nullptr);
    ~optionsDialog();
signals:
    void signalSaveChanges();
private slots:
    void on_applyPushButton_clicked();
    void changeDialog();
private:

    std::shared_ptr<xmlElem> allStages;
    Ui::optionsDialog *ui;
};

#endif // OPTIONSDIALOG_H
