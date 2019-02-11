#ifndef WIDGETTAB_H
#define WIDGETTAB_H

#include <QWidget>
#include <memory>

struct xmlElem;
class QSpinBox;
class QDoubleSpinBox;
class QSlider;
class QCheckBox;
class QLabel;

namespace Ui {
class WidgetItemTab;
class WidgetBulletTab;
}

class AbstractWidgetTab: public QWidget
{
    Q_OBJECT
public:
    explicit AbstractWidgetTab(QWidget* parent = nullptr);
    ~AbstractWidgetTab() override;
    std::shared_ptr<xmlElem> prepareData() const;
    bool needLoadData() const;

signals:
    void signalChanged();
protected slots:
    void slotChanged();

private:
    virtual std::shared_ptr<xmlElem> readData() const = 0;
protected:
    bool ifChanged;
};


class WidgetItemTab : public AbstractWidgetTab
{
    Q_OBJECT

public:
    explicit WidgetItemTab(std::shared_ptr<xmlElem> data,
                           std::shared_ptr<xmlElem> gameObjectData,
                           QWidget *parent = nullptr);
    ~WidgetItemTab() override;

private slots:
    void slotChangeIsStatic(int newValue);
    void slotChangePixmap();
private:
    std::shared_ptr<xmlElem> readData() const override;
    bool setNewPixmap(const QString& pixmapPath);

    Ui::WidgetItemTab* ui;
    QString pixmapPath;
};


class WidgetBulletTab: public AbstractWidgetTab
{
    Q_OBJECT
public:
    explicit WidgetBulletTab(std::shared_ptr<xmlElem> data,
                             std::shared_ptr<xmlElem> bulletData,
                             std::shared_ptr<xmlElem> weaponParam,
                             QWidget* parent = nullptr);
    ~WidgetBulletTab() override;

private slots:
    void slotChangeIsStatic(int newValue);
    void slotChangePixmap();
private:
    std::shared_ptr<xmlElem> readData() const override;
    std::shared_ptr<xmlElem> readBulletData() const;
    std::shared_ptr<xmlElem> readWeaponData() const;

    bool setNewPixmap(const QString& pixmapPath);

    Ui::WidgetBulletTab* ui;
    QString pixmapPath;
};


#endif // WIDGETTAB_H
