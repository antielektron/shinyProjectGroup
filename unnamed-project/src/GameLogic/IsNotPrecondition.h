#ifndef UNNAMED_PROJECT_ISNOTPRECONDITION_H
#define UNNAMED_PROJECT_ISNOTPRECONDITION_H

#include "GameLogic/PreconditionBase.h"

class IsNotPrecondition : public PreconditionBase
{
public:
    IsNotPrecondition(GlobalState *state, PreconditionBase *condition);
    virtual ~IsNotPrecondition();

    virtual bool evaluateCondition() override;

protected:
    PreconditionBase *m_condition;
};

#endif // ISNOTPRECONDITION_H
