#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_CONJUNCTION_PRECONDITION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_CONJUNCTION_PRECONDITION_H

#include "GameLogic/Preconditions/BooleanPreconditionBase.h"
#include "GameLogic/Traits.h"

class GlobalState;

class ConjunctionPrecondition : public BooleanPreconditionBase
{
public:
    ConjunctionPrecondition();
    ConjunctionPrecondition(GlobalState *state, const QDomElement &domElement);
    virtual ~ConjunctionPrecondition();

    virtual bool evaluateCondition() override;

    virtual QString type() override;
};


namespace traits
{

    template <>
    struct precondition_name<ConjunctionPrecondition>
    {
        static constexpr const char *value = "and";
    };

}


#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_CONJUNCTION_PRECONDITION_H
