#ifndef MODELLISTITEM_H
#define MODELLISTITEM_H

#include <QList>
#include <QVariant>

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

    /**
     * returns the object's identifier as a QString
     * (analog to the data() function in the Qt examples)
     * @return object's identifier as a QString
     */
    QVariant getData();

    ObjectBase *getObject();

    ModelListItem *getParent();

protected:

    QList<ModelListItem*> m_childItems;
    ModelListItem* m_parent;

    ObjectBase* m_object;

    int m_depth;

    QString m_objectName;
};

#endif // MODELLISTITEM_H
