#ifndef CREATENEWOBJECTSPARAMDIALOG_H
#define CREATENEWOBJECTSPARAMDIALOG_H

#include <QDialog>

namespace Ui {
class CreateNewObjectsParamDialog;
}

class CreateNewObjectsParamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNewObjectsParamDialog(QWidget *parent = nullptr);
    ~CreateNewObjectsParamDialog();
signals:
    void newNameOfSettings(QString);

public slots:
    void goodNameOfSettings();

private slots:
    void textEdited(QString curText);
    void on_createPushButton_clicked();


private:
    Ui::CreateNewObjectsParamDialog *ui;
};

#endif // CREATENEWOBJECTSPARAMDIALOG_H
