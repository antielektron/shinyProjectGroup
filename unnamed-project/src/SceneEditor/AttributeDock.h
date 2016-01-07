#ifndef ATTRIBUTEDOCK_H
#define ATTRIBUTEDOCK_H

#include <QDockWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "SceneEditor/GlobalAttributesTableWidget.h"
#include "GameLogic/GlobalState.h"


class AttributeDock : public QWidget
{
public:
    AttributeDock(QWidget *parent = nullptr);

protected slots:
    void onAddBoolClicked();
    void onAddFloatClicked();
    void onAddIntClicked();
    void onAddVectorClicked();

signals:
    void attributeAdded(const QString &key);

protected:
    void generateWidgets();
    void connectStuff();

    GlobalAttributesTableWidget *m_attributeTable;

    QPushButton *m_addBool;
    QPushButton *m_addInt;
    QPushButton *m_addFloat;
    QPushButton *m_addVector;

    QHBoxLayout *m_buttonRowLayout;
    QVBoxLayout *m_vLayout;

};

#endif // ATTRIBUTEDOCK_H
