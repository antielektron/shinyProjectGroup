#ifndef UNNAMED_PROJECT_ISANDPRECONDITION_H
#define UNNAMED_PROJECT_ISANDPRECONDITION_H

#include "GameLogic/PreconditionBase.h"

class IsAndPrecondition : public PreconditionBase
{
public:
    IsAndPrecondition(GlobalState *state,
                      PreconditionBase *baseA,
                      PreconditionBase *baseB);

    virtual ~IsAndPrecondition();

    virtual bool evaluateCondition() override;

protected:
    PreconditionBase *m_baseA;
    PreconditionBase *m_baseB;
};

#endif // UNNAMED_PROJECT_ISANDPRECONDITION_H
