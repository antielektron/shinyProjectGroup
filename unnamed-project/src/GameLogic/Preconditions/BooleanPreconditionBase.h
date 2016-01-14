#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BOOLEAN_PRECONDITION_BASE_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BOOLEAN_PRECONDITION_BASE_H

#include "GameLogic/Preconditions/PreconditionBase.h"
#include <memory>
#include <vector>

class BooleanPreconditionBase : public PreconditionBase
{
public:
    BooleanPreconditionBase(GlobalState *state);
    virtual ~BooleanPreconditionBase() {}

    void addCondition(std::unique_ptr<PreconditionBase> condition);
    void removeCondition(size_t i);

    // TODO begin/end

    virtual QString string() override;

protected:
    std::vector<std::unique_ptr<PreconditionBase>> m_conditions;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BOOLEAN_PRECONDITION_BASE_H
