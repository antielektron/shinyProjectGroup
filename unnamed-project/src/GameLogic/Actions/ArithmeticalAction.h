#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_ARITHMETICAL_ACTION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_ARITHMETICAL_ACTION_H

#include "GameLogic/Actions/ActionBase.h"
#include "GameLogic/GameLogicDatatypes.h"

class QVariant;

template<class T>
class ArithmeticalAction : public ActionBase
{
public:
    ArithmeticalAction(GlobalState *globalState,
                       const QString &valA,
                       const QString &valB,
                       const QString &valDst,
                       ArithmeticalOperationType type);

    virtual ~ArithmeticalAction();

    virtual void performAction() override;

    virtual ActionType getActionType() override;

    virtual QString getDataType() override;

    ArithmeticalOperationType getOperationType();

    const QString &getLeftOperandKey();
    const QString &getRightOperandKey();
    const QString &getDestKey();

protected:
    T getValueFromQVariant(QVariant v);

    QString m_valA;
    QString m_valB;
    QString m_valDst;

    ArithmeticalOperationType m_type;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_ARITHMETICAL_ACTION_H
