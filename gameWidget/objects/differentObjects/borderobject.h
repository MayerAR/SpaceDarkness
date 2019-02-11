#ifndef BORDEROBJECT_H
#define BORDEROBJECT_H

#include  "gameWidget/objects/object.h"


class BorderObject: public Object
{
public:
    BorderObject(const QString& pixmapPath, const QSize& size, objectID curID_, strategyID strID_);
    ~BorderObject() override;
    bool ifAlive() const override;
protected:
    void doUpdate(const std::shared_ptr<ChangeObjectData> upData) override;
};

#endif // BORDEROBJECT_H
