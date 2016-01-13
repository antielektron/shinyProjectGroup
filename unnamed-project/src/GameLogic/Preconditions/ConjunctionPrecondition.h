#ifndef UNNAMED_PROJECT_PRECONDITIONS_CONJUNCTION_PRECONDITION_H
#define UNNAMED_PROJECT_PRECONDITIONS_CONJUNCTION_PRECONDITION_H

#include "GameLogic/Preconditions/PreconditionBase.h"
#include <memory>

class ConjunctionPrecondition : public PreconditionBase
{
public:
    ConjunctionPrecondition(GlobalState *state,
                              std::unique_ptr<PreconditionBase> baseA,
                              std::unique_ptr<PreconditionBase> baseB);

    virtual ~ConjunctionPrecondition();

    virtual bool evaluateCondition() override;

    virtual QString toQString() override;

protected:
    std::unique_ptr<PreconditionBase> m_baseA;
    std::unique_ptr<PreconditionBase> m_baseB;
};

#endif // UNNAMED_PROJECT_PRECONDITIONS_CONJUNCTION_PRECONDITION_H
