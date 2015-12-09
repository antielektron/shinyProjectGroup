#include "ObjectListItem.h"
#include "Scene/ObjectBase.h"
ObjectListItem::ObjectListItem(ObjectBase *object, ObjectListItem *parent)
{
    m_parent = parent;
    m_object = object;

    m_depth = 0;

    if (m_parent)
    {
        m_depth = m_parent->getDepth() + 1;
    }
}

ObjectListItem::~ObjectListItem()
{
    qDeleteAll(m_childItems);
}

void ObjectListItem::appendChild(ObjectListItem *child)
{
    m_childItems.append(child);
}

ObjectListItem *ObjectListItem::getChild(int index)
{
    return m_childItems.value(index);
}

int ObjectListItem::getNumberOfChilds()
{
    return m_childItems.count();
}

int ObjectListItem::getRow()
{
    if (m_parent)
    {
        return m_parent->m_childItems.indexOf(const_cast<ObjectListItem*>(this));
    }
    return 0;
}

int ObjectListItem::getDepth()
{
    return m_depth;
}

QVariant ObjectListItem::getData()
{
    return m_object->getName();
}

ObjectListItem *ObjectListItem::getParent()
{
    return m_parent;
}

ObjectBase *ObjectListItem::getObject()
{
    return m_object;
}

