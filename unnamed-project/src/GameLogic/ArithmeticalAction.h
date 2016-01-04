#ifndef ARITHMETICALACTION_H
#define ARITHMETICALACTION_H

#include <QString>
#include "GameLogic/ActionBase.h"

enum class ArithmeticalOperationType
{
    additionType,
    subtractionType,
    multiplicationType,
    divisionType
};

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

#endif // ARITHMETICALACTION_H
