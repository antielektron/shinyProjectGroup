#include "SceneEditor/AttributesTableWidget.h"
#include "SceneEditor/SceneEditorGame.h"
#include "GameLogic/GlobalState.h"

// DEBUG
#include <iostream>

//------------------------------------------------------------------------------
AttributesTableWidget::AttributesTableWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) :
        QTableView(parent),
        m_game(game)
{
    m_model = new QStandardItemModel(0, 3, this);

    m_model->setHorizontalHeaderItem(0, new QStandardItem(QString("Key")));
    m_model->setHorizontalHeaderItem(1, new QStandardItem(QString("Type")));
    m_model->setHorizontalHeaderItem(2, new QStandardItem(QString("Value")));

    this->setModel(m_model);
    this->setShowGrid(false);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connectStuff();

    rebuildTable();
}

//------------------------------------------------------------------------------
AttributesTableWidget::~AttributesTableWidget()
{}

//------------------------------------------------------------------------------
void AttributesTableWidget::connectStuff()
{
    connect(m_game.get(), SIGNAL(attributesChanged()), this, SLOT(onAttributesChanged()));
    connect(m_game.get(), SIGNAL(sceneChanged()), this, SLOT(onAttributesChanged()));
}

//------------------------------------------------------------------------------
void AttributesTableWidget::onAttributesChanged()
{
    rebuildTable();
}

//------------------------------------------------------------------------------
void AttributesTableWidget::addItem(const QString &key, QMetaType::Type type, const QString &value)
{
    QList<QStandardItem *> list;
    list.append(new QStandardItem(key));
    list.append(new QStandardItem(QMetaType::typeName(type)));
    list.append(new QStandardItem(value));

    m_model->appendRow(list);

    // i hope the item pointers are handled by the model, if not this is a
    // possible memory leak. The examples i found don't care about it
    // so i hope those authors know what they're doing!

    // NOTE: you give the ownership to the model. so in fact you should not care.
}

//------------------------------------------------------------------------------
void AttributesTableWidget::rebuildTable()
{
    m_model->clear();

    auto globalState = m_game->getGlobalState();

    if (globalState != nullptr)
    {
        m_model->setRowCount(0);
        m_model->setColumnCount(3);

        m_model->setHorizontalHeaderItem(0, new QStandardItem(QString("Key")));
        m_model->setHorizontalHeaderItem(1, new QStandardItem(QString("Type")));
        m_model->setHorizontalHeaderItem(2, new QStandardItem(QString("Value")));

        for (const auto &attr : globalState->getAttributes())
        {
            QString key = attr.first;
            QString val = attributeToString(attr.second);
            auto type = (QMetaType::Type)attr.second.type();
            addItem(key, type, val);
        }
    }
}

//------------------------------------------------------------------------------
QString AttributesTableWidget::attributeToString(const QVariant &attr)
{
    switch((QMetaType::Type)attr.type())
    {
        case QMetaType::Bool:
        {
            return QString(attr.toBool() ? "true" : "false");
        }
        case QMetaType::Int:
        {
            return QString::number(attr.toInt());
        }
        case QMetaType::Float:
        {
            return QString::number(attr.toFloat());
        }
        case QMetaType::QVector3D:
        {
            QVector3D val = attr.value<QVector3D>();
            return QString("(") +
                    QString::number(val[0]) + ", " +
                    QString::number(val[1]) + ", " +
                    QString::number(val[2]) + ")";
        }
        default:
        {
            return "UNKNOWN";
        }
    }
}
