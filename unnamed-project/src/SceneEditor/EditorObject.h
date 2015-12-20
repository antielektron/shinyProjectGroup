#ifndef EDITOROBJECT_H
#define EDITOROBJECT_H

#include "Scene/Object.h"

class EditorObject : public Object
{
public:
    EditorObject(Model *model);
    virtual ~EditorObject();

    virtual ObjectType getObjectType() override;

    void makeVisible();
    void makeInvisible();
    bool isVisible();

protected:
    bool m_visible;
};

#endif // EDITOROBJECT_H
