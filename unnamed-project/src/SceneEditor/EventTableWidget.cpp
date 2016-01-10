#include "SceneEditor/EventTableWidget.h"

#include <QStandardItem>
#include <QString>
#include <QList>

#include "GameLogic/GameLogicDatatypes.h"
#include "GameLogic/CopyAttributeAction.h"
#include "GameLogic/FlipBooleanAction.h"
#include "GameLogic/ArithmeticalAction.h"

//------------------------------------------------------------------------------
EventTableWidget::EventTableWidget(QWidget *parent) : QTableView(parent)
{
    init();
}

//------------------------------------------------------------------------------
void EventTableWidget::init()
{
    m_model = new QStandardItemModel(0,3,this);

    m_model->setHorizontalHeaderItem(0, new QStandardItem(QString("event key")));
    m_model->setHorizontalHeaderItem(1, new QStandardItem(QString("precondition")));
    m_model->setHorizontalHeaderItem(2, new QStandardItem(QString("action")));

    setModel(m_model);

    setShowGrid(false);

    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

//------------------------------------------------------------------------------
void EventTableWidget::addItem(const QString &eventKey,
                               PreconditionBase *precondition,
                               ActionBase *action)
{
    QList<QStandardItem *> list;

    list.append(new QStandardItem(eventKey));
    list.append(new QStandardItem(precondition->toQString()));
    list.append(new QStandardItem(actionToQString(action)));

    m_model->appendRow(list);
}

//------------------------------------------------------------------------------
void EventTableWidget::onEventsChanged(GlobalState *globalState)
{
    rebuidList(globalState);
}

//------------------------------------------------------------------------------
QString EventTableWidget::actionToQString(ActionBase *action)
{
    ActionType actionType = action->getActionType();
    switch(actionType)
    {
    case ActionType::CopyAttribute:
    {
        CopyAttributeAction *copyAction = static_cast<CopyAttributeAction *>(action);
        return QString("copy '") + copyAction->getSourceKey()
                + QString("' to '") + copyAction->getDestKey()
                + QString("'");
    }
    case ActionType::FlipBoolean:
    {
        FlipBooleanAction *flipAction = static_cast<FlipBooleanAction *>(action);
        return QString("flip '") + flipAction->getKey() + QString("'");
    }
    case ActionType::ArithmeticalAction:
    {
        QString t = action->getDataType();
        switch (qStringToType.at(t))
        {
        case AttributeDatatype::Float:
        {
            ArithmeticalAction<float> *arithAction
                    = static_cast<ArithmeticalAction<float> *>(action);
            return QString("'") + arithAction->getDestKey()
                    + QString("' = '") + arithAction->getLeftOperandKey()
                    + QString("' ")
                    + arithOperationTypeToQString.at(arithAction->getOperationType())
                    + QString(" '") + arithAction->getRightOperandKey()
                    + QString("'");
        }
        case AttributeDatatype::Int:
        {
            ArithmeticalAction<int> *arithAction
                    = static_cast<ArithmeticalAction<int> *>(action);
            return QString("'") + arithAction->getDestKey()
                    + QString("' = '") + arithAction->getLeftOperandKey()
                    + QString("' ")
                    + arithOperationTypeToQString.at(arithAction->getOperationType())
                    + QString(" '") + arithAction->getRightOperandKey()
                    + QString("'");
        }
        case AttributeDatatype::Bool:
        case AttributeDatatype::QVector3D:
        {
            // nothing to do here...
        }
        }
    }
    }
    return QString("?"); // will theoretically never reached
}

//------------------------------------------------------------------------------
void EventTableWidget::rebuidList(GlobalState *globalState)
{
    m_model->clear();

    m_model->setRowCount(0);
    m_model->setColumnCount(3);

    m_model->setHorizontalHeaderItem(0, new QStandardItem(QString("event key")));
    m_model->setHorizontalHeaderItem(1, new QStandardItem(QString("precondition")));
    m_model->setHorizontalHeaderItem(2, new QStandardItem(QString("action")));

    for (const auto &event : globalState->getEvents())
    {
        QString eventKey = event.first;
        PreconditionBase *precondition = event.second.first.get();
        ActionBase *action = event.second.second.get();

        addItem(eventKey, precondition, action);
    }
}
