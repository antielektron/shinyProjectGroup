#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_NEGATION_PRECONDITION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_NEGATION_PRECONDITION_H

#include <memory>

#include "GameLogic/Preconditions/PreconditionBase.h"

class NegationPrecondition : public PreconditionBase
{
public:
    NegationPrecondition(GlobalState *state, std::unique_ptr<PreconditionBase> condition);
    virtual ~NegationPrecondition();

    virtual bool evaluateCondition() override;

    virtual QString toQString() override;

protected:
    std::unique_ptr<PreconditionBase> m_condition;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_NEGATION_PRECONDITION_H
