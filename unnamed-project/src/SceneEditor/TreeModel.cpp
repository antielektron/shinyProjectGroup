#include "TreeModel.h"
#include "SceneEditor/ObjectListItem.h"
#include "Scene/ObjectGroup.h"
#include "Scene/Object.h"
#include <memory>

TreeModel::TreeModel(ObjectGroup *rootObject, QObject *parent) : QAbstractItemModel(parent)
{
    m_rootObjectGroup = rootObject;
    m_rootItem.reset(new ObjectListItem(m_rootObjectGroup));
    setupModelData(m_rootItem.get());
}

//just implemented as explained by:
//http://doc.qt.io/qt-5/qtwidgets-itemviews-simpletreemodel-example.html
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    ObjectListItem *parentItem;

    if (!parent.isValid())
    {
        parentItem = m_rootItem.get();
    }
    else
    {
        parentItem = static_cast<ObjectListItem*>(parent.internalPointer());
    }

    ObjectListItem *childItem = parentItem->getChild(row);

    if (childItem)
    {
        return createIndex(row,column, childItem);
    }

    //else:
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    ObjectListItem *childItem = static_cast<ObjectListItem*>(index.internalPointer());
    ObjectListItem *parentItem = childItem->getParent();

    if (parentItem == m_rootItem.get())
    {
        return QModelIndex();
    }

    return createIndex(parentItem->getRow(),0,parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    ObjectListItem *parentItem;

    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = m_rootItem.get();
    }
    else
    {
        parentItem = static_cast<ObjectListItem* >(parent.internalPointer());
    }

    return parentItem->getNumberOfChilds();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
    /*
    if (parent.isValid())
    {
        return static_cast<ModelListItem* >(parent.internalPointer())->getDepth();
    }
    return m_rootItem->getDepth();
    */
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    ObjectListItem *item = static_cast<ObjectListItem *>(index.internalPointer());

    return item->getData();

}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return 0;
    }

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
       return m_rootItem->getData(); //FIXME: not sure if this
                                     //results in the behaviour i want
    }

    return QVariant();
}

void TreeModel::setRoot(ObjectGroup *rootObject)
{
    m_rootObjectGroup = rootObject;
    m_rootItem.reset(new ObjectListItem(m_rootObjectGroup));
    setupModelData(m_rootItem.get());
}

void TreeModel::setupModelData(ObjectListItem *rootItem)
{
    ObjectGroup *rootGroup = static_cast<ObjectGroup*>(rootItem->getObject());

    //iterate through child groups:
    for (const auto &objGroup : rootGroup->getGroups())
    {
        ObjectListItem *childGroup = new ObjectListItem(objGroup, rootItem);
        rootItem->appendChild(childGroup);
        setupModelData(childGroup);
    }

    //iterate through child objects:
    for (const auto &obj : rootGroup->getObjects())
    {
        ObjectListItem *childObject = new ObjectListItem(obj, rootItem);
        rootItem->appendChild(childObject);
    }
}


