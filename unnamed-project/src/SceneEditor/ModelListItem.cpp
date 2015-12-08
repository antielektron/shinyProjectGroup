#include "ModelListItem.h"
#include "Scene/ObjectBase.h"
ModelListItem::ModelListItem(ObjectBase *object, ModelListItem *parent)
{
    m_parent = parent;
    m_object = object;

    m_depth = 0;

    if (m_parent)
    {
        m_depth = m_parent->getDepth() + 1;
    }
}

ModelListItem::~ModelListItem()
{
    qDeleteAll(m_childItems);
}

void ModelListItem::appendChild(ModelListItem *child)
{
    m_childItems.append(child);
}

ModelListItem *ModelListItem::getChild(int index)
{
    return m_childItems.value(index);
}

int ModelListItem::getNumberOfChilds()
{
    return m_childItems.count();
}

int ModelListItem::getIndex()
{
    if (m_parent)
    {
        return m_parent->m_childItems.indexOf(const_cast<ModelListItem*>(this));
    }
    return 0;
}

int ModelListItem::getDepth()
{
    return m_depth;
}

QVariant ModelListItem::getData()
{
    return m_object->getName();
}

ModelListItem *ModelListItem::getParent()
{
    return m_parent;
}

ObjectBase *ModelListItem::getObject()
{
    return m_object;
}

