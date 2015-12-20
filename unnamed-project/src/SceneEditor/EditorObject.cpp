#include "EditorObject.h"

EditorObject::EditorObject(Model *model) : Object(model)
{
    m_visible = true;
}

//------------------------------------------------------------------------------
EditorObject::~EditorObject()
{

}

//------------------------------------------------------------------------------
void EditorObject::makeInvisible()
{
    m_visible = false;
}

//------------------------------------------------------------------------------
void EditorObject::makeVisible()
{
    m_visible = true;
}

//------------------------------------------------------------------------------
bool EditorObject::isVisible()
{
    return m_visible;
}

//------------------------------------------------------------------------------
ObjectType EditorObject::getObjectType()
{
    return ObjectType::EditorObject;
}
