#ifndef UNNAMED_PROJECT_GLOBALATTRIBUTESTABLEWIDGET_H
#define UNNAMED_PROJECT_GLOBALATTRIBUTESTABLEWIDGET_H

#include <QObject>
#include <QTableView>
#include <QStandardItemModel>
#include "GameLogic/GlobalState.h"

class GlobalAttributesTableWidget : public QTableView
{
    Q_OBJECT
public:
    GlobalAttributesTableWidget(QWidget *parent = nullptr);
    GlobalAttributesTableWidget(GlobalState *globalState,
                                QWidget *parent = nullptr);
    void init();

    void addItem(const QString &key,
                 AttributeDatatype type,
                 const QString &value);

public slots:
    void onAttributesChanged(GlobalState *globalState);
    void onSingleAttributeAdded(GlobalState *globalState, const QString &key);

signals:

protected:
    void rebuildList(GlobalState *globalState);

    QString attributeToString(const QVariant &attr,AttributeDatatype type);

    QStandardItemModel *m_model;

};

#endif // UNNAMED_PROJECT_GLOBALATTRIBUTESTABLEWIDGET_H
