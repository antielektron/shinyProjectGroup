#ifndef MODELLISTITEM_H
#define MODELLISTITEM_H

#include <QList>
#include <QVariant>

class ObjectBase;

class ObjectListItem
{
public:

    ObjectListItem(ObjectBase *object, ObjectListItem *parent = nullptr);
    ~ObjectListItem();

    void appendChild(ObjectListItem *child);
    ObjectListItem *getChild(int index);

    int getNumberOfChilds();
    int getRow();
    int getDepth();

    /**
     * returns the object's identifier as a QString
     * (analog to the data() function in the Qt examples)
     * @return object's identifier as a QString
     */
    QVariant getData();

    ObjectBase *getObject();

    ObjectListItem *getParent();

protected:

    QList<ObjectListItem*> m_childItems;
    ObjectListItem* m_parent;

    ObjectBase* m_object;

    int m_depth;
};

#endif // MODELLISTITEM_H
