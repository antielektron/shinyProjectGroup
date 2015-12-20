#ifndef EDITOROBJECT_H
#define EDITOROBJECT_H

#include "Scene/Object.h"

class EditorObject : public Object
{
public:
    EditorObject(Model *model);
    virtual ~EditorObject();

    virtual ObjectType getObjectType() override;

signals:

public slots:
};

#endif // EDITOROBJECT_H
