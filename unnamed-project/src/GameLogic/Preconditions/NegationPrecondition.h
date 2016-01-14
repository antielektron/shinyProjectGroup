#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_NEGATION_PRECONDITION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_NEGATION_PRECONDITION_H

#include <memory>

#include "GameLogic/Preconditions/PreconditionBase.h"
#include "GameLogic/Preconditions/Traits.h"

class NegationPrecondition : public PreconditionBase
{
public:
    NegationPrecondition(std::unique_ptr<PreconditionBase> condition);
    virtual ~NegationPrecondition();

    virtual bool evaluateCondition() override;

    virtual QString string() override;

    virtual QString name() override;

protected:
    std::unique_ptr<PreconditionBase> m_condition;
};


namespace traits
{

    template <>
    struct precondition_name<NegationPrecondition>
    {
        static constexpr const char *value = "neg";
    };

}


#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_NEGATION_PRECONDITION_H
