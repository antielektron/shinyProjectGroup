#ifndef UNNAMED_PROJECT_LOGICOBJECT_H
#define UNNAMED_PROJECT_LOGICOBJECT_H

#include "Scene/Object.h"

class LogicObject : public Object
{
public:
    LogicObject(Model *model);
    virtual ~LogicObject();

    virtual ObjectType getObjectType() override;

};

#endif // UNNAMED_PROJECT_LOGICOBJECT_H
