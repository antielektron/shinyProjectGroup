#ifndef UNNAMED_PROJECT_ISNOTPRECONDITION_H
#define UNNAMED_PROJECT_ISNOTPRECONDITION_H

#include "GameLogic/PreconditionBase.h"
#include <memory>

class IsNotPrecondition : public PreconditionBase
{
public:
    IsNotPrecondition(GlobalState *state, std::unique_ptr<PreconditionBase> condition);
    virtual ~IsNotPrecondition();

    virtual bool evaluateCondition() override;

    virtual QString toQString() override;

protected:
    std::unique_ptr<PreconditionBase> m_condition;
};

#endif // ISNOTPRECONDITION_H
