#include "TreeModel.h"
#include "ModelListItem.h"
#include <memory>

TreeModel::TreeModel()
{
    m_rootItem.reset();
}

//just implemented as explained by:
//http://doc.qt.io/qt-5/qtwidgets-itemviews-simpletreemodel-example.html
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    ModelListItem *parentItem;

    if (!parent.isValid())
    {
        parentItem = m_rootItem.get();
    }
    else
    {
        parentItem = static_cast<ModelListItem*>(parent.internalPointer());
    }

    ModelListItem *childItem = parentItem->getChild(row);

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

    ModelListItem *childItem = static_cast<ModelListItem*>(index.internalPointer());
    ModelListItem *parentItem = childItem->getParent();

    if (parentItem == m_rootItem.get())
    {
        return QModelIndex();
    }

    return createIndex(parentItem->getIndex(),0,parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    ModelListItem *parentItem;

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
        parentItem = static_cast<ModelListItem* >(parent.internalPointer());
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

    ModelListItem *item = static_cast<ModelListItem *>(index.internalPointer());

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


