#ifndef UNNAMED_PROJECT_ISORPRECONDITION_H
#define UNNAMED_PROJECT_ISORPRECONDITION_H

#include "GameLogic/PreconditionBase.h"
#include <memory>

class IsOrPrecondition : public PreconditionBase
{
public:
    IsOrPrecondition(GlobalState *state,
                     std::unique_ptr<PreconditionBase> baseA,
                     std::unique_ptr<PreconditionBase> baseB);
    virtual ~IsOrPrecondition();

    virtual bool evaluateCondition() override;

    virtual QString toQString() override;

protected:
    std::unique_ptr<PreconditionBase> m_baseA;
    std::unique_ptr<PreconditionBase> m_baseB;
};

#endif // UNNAMED_PROJECT_ISORPRECONDITION_H
