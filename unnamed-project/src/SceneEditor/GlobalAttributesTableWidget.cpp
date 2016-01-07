#include "GlobalAttributesTableWidget.h"
#include "GameLogic/GlobalState.h"

#include <QStandardItem>
#include <QString>
#include <QList>

#include "GameLogic/GameLogicDatatypes.h"

GlobalAttributesTableWidget::GlobalAttributesTableWidget(QWidget *parent) :
    QTableView(parent)
{
    init();
}

//------------------------------------------------------------------------------
GlobalAttributesTableWidget::GlobalAttributesTableWidget(
        GlobalState *globalState,
        QWidget *parent) :
    QTableView(parent)
{
    init();
    rebuildList(globalState);
}

//------------------------------------------------------------------------------
void GlobalAttributesTableWidget::init()
{

    m_model = new QStandardItemModel(0,3,this);

    m_model->setHorizontalHeaderItem(0, new QStandardItem(QString("key")));
    m_model->setHorizontalHeaderItem(1, new QStandardItem(QString("type")));
    m_model->setHorizontalHeaderItem(2, new QStandardItem(QString("value")));

    setModel(m_model);

    setShowGrid(true);
}

//------------------------------------------------------------------------------
void GlobalAttributesTableWidget::addItem(const QString &key,
                                          AttributeDatatype type,
                                          const QString &value)
{
    QList<QStandardItem *> list;
    list.append(new QStandardItem(key));
    list.append(new QStandardItem(typeToQString.at(type)));
    list.append(new QStandardItem(value));

    m_model->appendRow(list);

    // i hope the item pointers are handled by the model, if not this is a
    // possible memory leak. The examples i found don't care about it
    // so i hope those authors know what they're doing!
}

//------------------------------------------------------------------------------
void GlobalAttributesTableWidget::onAttributesChanged(GlobalState *globalState)
{
    rebuildList(globalState);
}

//------------------------------------------------------------------------------
void GlobalAttributesTableWidget::onSingleAttributeAdded(
        GlobalState *globalState,
        const QString &key)
{
    AttributeDatatype type = globalState->getType(key);
    QString val = attributeToString(globalState->getValue(key),
                                    type);

    addItem(key, type, val);
}

//------------------------------------------------------------------------------
void GlobalAttributesTableWidget::rebuildList(GlobalState *globalState)
{
    m_model->clear();

    m_model->setRowCount(0);
    m_model->setColumnCount(3);

    m_model->setHorizontalHeaderItem(0, new QStandardItem(QString("key")));
    m_model->setHorizontalHeaderItem(1, new QStandardItem(QString("type")));
    m_model->setHorizontalHeaderItem(2, new QStandardItem(QString("value")));

    for (const auto &attr : globalState->getAttributes())
    {
        QString key = attr.first;
        AttributeDatatype type = globalState->getType(key);
        QString val = attributeToString(globalState->getValue(key),
                                        type);

        addItem(key,type,val);
    }
}

//------------------------------------------------------------------------------
QString GlobalAttributesTableWidget::attributeToString(const QVariant &attr,
                                                       AttributeDatatype type)
{
    switch(type)
    {
    case AttributeDatatype::Bool:
    {
        return QString(attr.toBool() ? "true" : "false");
    }
    case AttributeDatatype::Int:
    {
        return QString("int");
    }
    case AttributeDatatype::Float:
    {
        return QString("float");
    }
    case AttributeDatatype::QVector3D:
    {
        return QString("vector");
    }
    }
    return "UNKNOWN";
}

