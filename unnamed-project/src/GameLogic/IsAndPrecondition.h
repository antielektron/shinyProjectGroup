#ifndef UNNAMED_PROJECT_ISANDPRECONDITION_H
#define UNNAMED_PROJECT_ISANDPRECONDITION_H

#include "GameLogic/PreconditionBase.h"
#include <memory>

class IsAndPrecondition : public PreconditionBase
{
public:
    IsAndPrecondition(GlobalState *state,
                      std::unique_ptr<PreconditionBase> baseA,
                      std::unique_ptr<PreconditionBase> baseB);

    virtual ~IsAndPrecondition();

    virtual bool evaluateCondition() override;

    virtual QString toQString() override;

protected:
    std::unique_ptr<PreconditionBase> m_baseA;
    std::unique_ptr<PreconditionBase> m_baseB;
};

#endif // UNNAMED_PROJECT_ISANDPRECONDITION_H
