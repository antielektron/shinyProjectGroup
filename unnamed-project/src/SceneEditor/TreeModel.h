#ifndef UNNAMED_PROJECT_TREEMODEL_H
#define UNNAMED_PROJECT_TREEMODEL_H

#include <QAbstractItemModel>
#include <memory>
#include <SceneEditor/ModelListItem.h>

class ObjectBase;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TreeModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

protected:
    std::unique_ptr<ModelListItem> m_rootItem;

    void setupModelData();

};

#endif // UNNAMED_PROJECT_TREEMODEL_H
