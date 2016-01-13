#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_CONJUNCTION_PRECONDITION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_CONJUNCTION_PRECONDITION_H

#include "GameLogic/Preconditions/BooleanPreconditionBase.h"

class ConjunctionPrecondition : public BooleanPreconditionBase
{
public:
    ConjunctionPrecondition(GlobalState *state);
    virtual ~ConjunctionPrecondition();

    virtual bool evaluateCondition() override;

    virtual QString toQString() override;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_CONJUNCTION_PRECONDITION_H
