#ifndef UNNAMED_PROJECT_SCENE_OBJECT_GROUP_H
#define UNNAMED_PROJECT_SCENE_OBJECT_GROUP_H

#include <memory>

#include "Scene/ObjectBase.h"
#include "smartiterator.h"

class Object;

class ObjectGroup : public ObjectBase
{
public:

    void addObject(Object* object);
    void addObjectGroup(ObjectGroup* group);

    void removeObject(Object *object);
    void removeObjectGroup(ObjectGroup *group);

    // NOT VIRTUAL (not because we forgot it, but we know what we do.)
    void updateWorld();

    // hide smartpointers from user.
    typedef std::vector<Object*>::iterator object_iterator_type;
    typedef std::vector<ObjectGroup*>::iterator group_iterator_type;

    range<object_iterator_type> getObjects();
    range<group_iterator_type> getGroups();

private:
    std::vector<Object*> m_objects;
    std::vector<ObjectGroup*> m_groups;
};

#endif // UNNAMED_PROJECT_SCENE_OBJECT_GROUP_H
