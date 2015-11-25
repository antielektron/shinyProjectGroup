#ifndef UNNAMED_PROJECT_SCENE_OBJECT_GROUP_H
#define UNNAMED_PROJECT_SCENE_OBJECT_GROUP_H

#include "Scene/ObjectBase.h"

class Object;

class ObjectGroup : public ObjectBase
{
public:

private:
    std::vector<std::unique_ptr<Object>> m_objects;
    std::vector<std::unique_ptr<ObjectGroup>> m_groups;
};

#endif // UNNAMED_PROJECT_SCENE_OBJECT_GROUP_H
