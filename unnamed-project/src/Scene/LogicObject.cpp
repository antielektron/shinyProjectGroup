#include "Scene/LogicObject.h"

//------------------------------------------------------------------------------
LogicObject::LogicObject(Model *model) : Object(model)
{

}

//------------------------------------------------------------------------------
LogicObject::~LogicObject()
{
    // nothing to do here
}

//------------------------------------------------------------------------------
ObjectType LogicObject::getObjectType()
{
    return ObjectType::LogicObject;
}
