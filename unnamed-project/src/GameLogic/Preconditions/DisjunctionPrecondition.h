#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_DISJUNCTION_PRECONDITION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_DISJUNCTION_PRECONDITION_H

#include "GameLogic/Preconditions/BooleanPreconditionBase.h"
#include "GameLogic/Preconditions/Traits.h"

class DisjunctionPrecondition : public BooleanPreconditionBase
{
public:
    DisjunctionPrecondition();
    virtual ~DisjunctionPrecondition();

    virtual bool evaluateCondition() override;

    virtual QString name() override;
};

namespace traits
{

    template <>
    struct precondition_name<DisjunctionPrecondition>
    {
        static constexpr const char *value = "or";
    };

}


#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_DISJUNCTION_PRECONDITION_H
