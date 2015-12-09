#ifndef UNNAMED_PROJECT_TREEMODEL_H
#define UNNAMED_PROJECT_TREEMODEL_H

#include <QAbstractItemModel>
#include <memory>
#include <SceneEditor/ObjectListItem.h>

class ObjectGroup;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TreeModel(ObjectGroup *rootObject, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    void setRoot(ObjectGroup *rootObject);

protected:
    std::unique_ptr<ObjectListItem> m_rootItem;

    ObjectGroup* m_rootObjectGroup;

    void setupModelData(ObjectListItem *rootItem);

};

#endif // UNNAMED_PROJECT_TREEMODEL_H
