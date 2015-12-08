#include "Scene/ObjectGroup.h"

#include "Scene/Object.h"

void ObjectGroup::updateWorld()
{
    ObjectBase::updateWorld();

    for (auto &obj : m_objects)
    {
        obj->updateWorld();
    }

    for (auto &obj : m_groups)
    {
        obj->updateWorld();
    }
}

void ObjectGroup::addObject(Object* object)
{
    m_objects.push_back(object);
}

void ObjectGroup::addObjectGroup(ObjectGroup* group)
{
    m_groups.push_back(group);
}

void ObjectGroup::removeObject(Object *object)
{
    // TODO consider using a set or sort the vector..
    for (auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        if (*it == object)
        {
            m_objects.erase(it);
            break;
        }
    }
}

void ObjectGroup::removeObjectGroup(ObjectGroup *group)
{
    for (auto it = m_groups.begin(); it != m_groups.end(); it++)
    {
        if (*it == group)
        {
            m_groups.erase(it);
            break;
        }
    }
}

range<ObjectGroup::object_iterator_type> ObjectGroup::getObjects()
{
    return range<object_iterator_type>(m_objects.begin(), m_objects.end());
}

range<ObjectGroup::group_iterator_type> ObjectGroup::getGroups()
{
    return range<group_iterator_type>(m_groups.begin(), m_groups.end());
}
