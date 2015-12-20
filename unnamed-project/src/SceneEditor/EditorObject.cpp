#include "EditorObject.h"

EditorObject::EditorObject(Model *model) : Object(model)
{

}

//------------------------------------------------------------------------------
EditorObject::~EditorObject()
{

}

//------------------------------------------------------------------------------
ObjectType EditorObject::getObjectType()
{
    return ObjectType::EditorObject;
}
