#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_DISJUNCTION_PRECONDITION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_DISJUNCTION_PRECONDITION_H

#include <memory>

#include "GameLogic/Preconditions/PreconditionBase.h"

class DisjunctionPrecondition : public PreconditionBase
{
public:
    DisjunctionPrecondition(GlobalState *state,
                             std::unique_ptr<PreconditionBase> baseA,
                             std::unique_ptr<PreconditionBase> baseB);
    virtual ~DisjunctionPrecondition();

    virtual bool evaluateCondition() override;

    virtual QString toQString() override;

protected:
    std::unique_ptr<PreconditionBase> m_baseA;
    std::unique_ptr<PreconditionBase> m_baseB;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_DISJUNCTION_PRECONDITION_H
