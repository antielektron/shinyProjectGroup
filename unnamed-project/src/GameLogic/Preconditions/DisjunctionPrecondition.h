#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_DISJUNCTION_PRECONDITION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_DISJUNCTION_PRECONDITION_H

#include "GameLogic/Preconditions/BooleanPreconditionBase.h"

class DisjunctionPrecondition : public BooleanPreconditionBase
{
public:
    DisjunctionPrecondition(GlobalState *state);
    virtual ~DisjunctionPrecondition();

    virtual bool evaluateCondition() override;

    virtual QString toQString() override;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_DISJUNCTION_PRECONDITION_H
