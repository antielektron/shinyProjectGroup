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

void ObjectGroup::addObject(std::unique_ptr<Object> object)
{
    m_objects.push_back(std::move(object));
}

void ObjectGroup::addObjectGroup(std::unique_ptr<ObjectGroup> group)
{
    m_groups.push_back(std::move(group));
}

void ObjectGroup::removeObject(Object *object)
{
    // TODO consider using a set or sort the vector..
    for (auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        if (it->get() == object)
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
        if (it->get() == group)
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
