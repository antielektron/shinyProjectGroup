#ifndef ATTRIBUTEWIDGET_H
#define ATTRIBUTEWIDGET_H

#include <QPushButton>
#include <QVBoxLayout>
#include "SceneEditor/GlobalAttributesTableWidget.h"
#include "GameLogic/GlobalState.h"


class AttributeWidget : public QWidget
{
    Q_OBJECT
public:
    AttributeWidget(QWidget *parent = nullptr);

protected slots:
    void onAddValueClicked();
    void onDelValueClicked();

public slots:
    void onAttributesChanged(GlobalState *globalState);
    void onSingleAttributeAdded(GlobalState *globalState, const QString &key);

signals:
    void attributeAdded(const QString &key,
                        QVariant value,
                        AttributeDatatype type);

    void attributeDeleted(const QString &key);

protected:
    void generateWidgets();
    void connectStuff();

    GlobalAttributesTableWidget *m_attributeTable;

    QPushButton *m_addValue;
    QPushButton *m_delValue;

    QVBoxLayout *m_vLayout;

};

#endif // ATTRIBUTEWIDGET_H
