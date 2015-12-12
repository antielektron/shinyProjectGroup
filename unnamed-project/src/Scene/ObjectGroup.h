#ifndef UNNAMED_PROJECT_SCENE_OBJECT_GROUP_H
#define UNNAMED_PROJECT_SCENE_OBJECT_GROUP_H

#include <memory>

#include "Scene/ObjectBase.h"
#include "smartiterator.h"

class Object;

class ObjectGroup : public ObjectBase
{
public:

    void addObject(std::unique_ptr<Object> object);
    void addObjectGroup(std::unique_ptr<ObjectGroup> group);

    void removeObject(Object *object);
    void removeObjectGroup(ObjectGroup *group);

    virtual ObjectType getObjectType();

    // NOT VIRTUAL (not because we forgot it, but we know what we do.)
    void updateWorld();

    // hide smartpointers from user.
    typedef smart_iterator<std::vector<std::unique_ptr<Object>>::iterator> object_iterator_type;
    typedef smart_iterator<std::vector<std::unique_ptr<ObjectGroup>>::iterator> group_iterator_type;

    range<object_iterator_type> getObjects();
    range<group_iterator_type> getGroups();

private:
    std::vector<std::unique_ptr<Object>> m_objects;
    std::vector<std::unique_ptr<ObjectGroup>> m_groups;
};

#endif // UNNAMED_PROJECT_SCENE_OBJECT_GROUP_H
