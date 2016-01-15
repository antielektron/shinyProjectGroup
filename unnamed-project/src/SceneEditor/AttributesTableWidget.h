#ifndef UNNAMED_PROJECT_SCENE_EDITOR_ATTRIBUTES_TABLE_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_ATTRIBUTES_TABLE_WIDGET_H

#include <memory>

#include <QTableView>
#include <QStandardItemModel>

class SceneEditorGame;

class AttributesTableWidget : public QTableView
{
    Q_OBJECT
public:
    AttributesTableWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent = nullptr);
    virtual ~AttributesTableWidget();

protected slots:
    void onAttributesChanged();

protected:
    void connectStuff();

    void addItem(const QString &key, QMetaType::Type type, const QString &value);
    void rebuildTable();

    QString attributeToString(const QVariant &attr);

    std::shared_ptr<SceneEditorGame> m_game;

    QStandardItemModel *m_model;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_ATTRIBUTES_TABLE_WIDGET_H
