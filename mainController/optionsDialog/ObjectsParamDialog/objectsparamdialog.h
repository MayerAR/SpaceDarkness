#ifndef OBJECTSPARAMDIALOG_H
#define OBJECTSPARAMDIALOG_H

#include <QDialog>
#include <QVector>
#include <memory>

struct xmlElem;
class AbstractWidgetTab;

namespace Ui {
class ObjectsParamDialog;
}

class ObjectsParamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectsParamDialog(QWidget *parent = nullptr);
    ~ObjectsParamDialog();

    bool loadTabs(const QString& name);
private slots:
    void slotChanged();
    void on_acceptPushButton_clicked();
private:
    void createAllFiles();

    void changeMistakeOnStandart(const QString& filePath, const QString& paramName);
    void deleteParam(const QString &paramName);
    void createParam();


    QString nameOfCurrentParam;
    QVector<AbstractWidgetTab*> allTabWidget;
    std::shared_ptr<xmlElem> objectParam;
    std::shared_ptr<xmlElem> gameObjectParam;
    std::shared_ptr<xmlElem> bulletParam;
    std::shared_ptr<xmlElem> weaponParam;
    Ui::ObjectsParamDialog *ui;
};

#endif // OBJECTSPARAMDIALOG_H
