#include "SceneEditor/EventWidget.h"
#include "GameLogic/GameLogicUtility.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QStringList>
#include <memory>

#include "GameLogic/CopyAttributeAction.h"
#include "GameLogic/FlipBooleanAction.h"
#include "GameLogic/ArithmeticalAction.h"

//------------------------------------------------------------------------------
EventWidget::EventWidget(QWidget *parent) :
    QWidget(parent)
{
    setGlobalState(nullptr);
    generateWidgets();
    connectStuff();
}

//------------------------------------------------------------------------------
void EventWidget::setGlobalState(GlobalState *globalState)
{
    m_globalState = globalState;
}

//------------------------------------------------------------------------------
void EventWidget::generateWidgets()
{
    m_vLayout = new QVBoxLayout(this);

    m_eventTable = new EventTableWidget(this);
    m_eventTable->setMinimumSize(200,100);

    m_addEvent = new QPushButton("Add Event", this);
    m_delEvent = new QPushButton("Del Event", this);

    m_vLayout->addWidget(m_eventTable);

    m_vLayout->addWidget(m_addEvent);
    m_vLayout->addWidget(m_delEvent);

    this->setLayout(m_vLayout);
}

//------------------------------------------------------------------------------
void EventWidget::connectStuff()
{
    connect(m_addEvent, SIGNAL(clicked()), this, SLOT(onAddEventClicked()));
    connect(m_delEvent, SIGNAL(clicked()), this, SLOT(onDelEventClicked()));
}

//------------------------------------------------------------------------------
void EventWidget::onEventsChanged()
{
    if (m_globalState)
    {
        m_eventTable->onEventsChanged(m_globalState);
    }
}

//------------------------------------------------------------------------------
void EventWidget::onAddEventClicked()
{
    if (!m_globalState)
    {
        QMessageBox::warning(this, "Warning", "Scene not initialized yet");
        return;
    }
    bool ok;

    QString eventKey =
            QInputDialog::getText(this,
                                  "enter event key",
                                  "key",
                                  QLineEdit::Normal,
                                  "key",
                                  &ok);

    if (!ok)
    {
        return;
    }

    QString preconditionString =
            QInputDialog::getText(this,
                                  "enter precondition string",
                                  "precondition",
                                  QLineEdit::Normal,
                                  "",
                                  &ok);
    if (!ok)
    {
        return;
    }

    // parse precondition:
    std::unique_ptr<PreconditionBase> precondition =
            gameLogicUtility::stringToPrecondition(
                m_globalState,
                preconditionString.toStdString());

    if (!precondition)
    {
        QMessageBox::critical(this, "Error", "error while parsing precondition");
        return;
    }

    // get action:

    // generate type list:
    QStringList strList;
    for (const auto &pair : qStringToAction)
    {
        strList.append(pair.first);
    }
    QString selectedActionType = QInputDialog::getItem(this,
                                                       "select action type",
                                                       "action type",
                                                       strList,
                                                       0,
                                                       false,
                                                       &ok);
    if (!ok)
    {
        return;
    }

    switch(qStringToAction.at(selectedActionType))
    {
    case ActionType::CopyAttribute:
    {
        QString src = QInputDialog::getText(this,
                                            "enter argument",
                                            "source key",
                                            QLineEdit::Normal,
                                            "",
                                            &ok);

        QString dst = QInputDialog::getText(this,
                                            "enter argument",
                                            "destination key",
                                            QLineEdit::Normal,
                                            "",
                                            &ok);

        auto actionPtr = std::unique_ptr<ActionBase>(
                             new CopyAttributeAction(m_globalState, src, dst));
        emit eventAdded(eventKey,
                        &precondition,
                        &actionPtr);
        return;
    }
    case ActionType::FlipBoolean:
    {
        QString key = QInputDialog::getText(this,
                                            "enter argument",
                                            "boolean key",
                                            QLineEdit::Normal,
                                            "",
                                            &ok);

        auto actionPtr = std::unique_ptr<ActionBase>(
                          new FlipBooleanAction(m_globalState, key));
        emit eventAdded(eventKey,
                        &precondition,
                        &actionPtr);
        return;
    }
    case ActionType::ArithmeticalAction:
    {
        QStringList typeList = {"float", "int"};
        QStringList operandList;
        for (const auto &pair : qStringToArithOperationType)
        {
            operandList.append(pair.first);
        }

        QString operandStr = QInputDialog::getItem(this,
                                                   "enter argument",
                                                   "operand type",
                                                   operandList,
                                                   0,
                                                   false,
                                                   &ok);

        QString dTypeStr = QInputDialog::getItem(this,
                                                 "enter argument",
                                                 "datatype",
                                                 typeList,
                                                 0,
                                                 false,
                                                 &ok);

        QString dst = QInputDialog::getText(this,
                                            "enter argument",
                                            "destination key",
                                            QLineEdit::Normal,
                                            "",
                                            &ok);

        QString a = QInputDialog::getText(this,
                                            "enter argument",
                                            "left operand key",
                                            QLineEdit::Normal,
                                            "",
                                            &ok);

        QString b = QInputDialog::getText(this,
                                            "enter argument",
                                            "right operand key",
                                            QLineEdit::Normal,
                                            "",
                                            &ok);

        ActionBase *action = nullptr;
        switch(qStringToType.at(dTypeStr))
        {
        case AttributeDatatype::Float:
        {
            action = new ArithmeticalAction<float>(
                               m_globalState,
                               a,
                               b,
                               dst,
                               qStringToArithOperationType.at(operandStr));
            break;
        }
        case AttributeDatatype::Int:
        {
            action = new ArithmeticalAction<int>(
                               m_globalState,
                               a,
                               b,
                               dst,
                               qStringToArithOperationType.at(operandStr));
        }
        case AttributeDatatype::Bool:
        case AttributeDatatype::QVector3D:
        {
            // nothing to do here
        }
        }
        std::unique_ptr<ActionBase> actionPtr(action);
        emit eventAdded(eventKey,
                        &precondition,
                        &actionPtr);
        return;
    }
    }

}

//------------------------------------------------------------------------------
void EventWidget::onDelEventClicked()
{
    QModelIndexList selectedList = m_eventTable->selectionModel()->selectedRows();
    for (int i = 0; i < selectedList.size(); i++)
    {
        emit eventDeleted(selectedList.at(i).data(0).toString());
    }
}

//------------------------------------------------------------------------------

