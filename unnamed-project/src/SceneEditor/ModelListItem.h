#ifndef MODELLISTITEM_H
#define MODELLISTITEM_H

#include <QList>

class ObjectBase;

class ModelListItem
{
public:

    ModelListItem(ObjectBase *object, ModelListItem *parent = nullptr);
    ~ModelListItem();

    void appendChild(ModelListItem *child);
    ModelListItem *getChild(int index);

    int getNumberOfChilds();
    int getIndex();
    int getDepth();

    ObjectBase *getObject();

    ModelListItem *getParent();

protected:

    QList<ModelListItem*> m_childItems;
    ModelListItem* m_parent;

    ObjectBase* m_object;

    int m_depth;
};

#endif // MODELLISTITEM_H
