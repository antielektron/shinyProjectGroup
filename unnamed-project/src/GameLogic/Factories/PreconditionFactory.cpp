#include "GameLogic/Factories/PreconditionFactory.h"

#include "GameLogic/Preconditions/IsEqualPrecondition.h"
#include "GameLogic/Preconditions/IsLessPrecondition.h"
#include "GameLogic/Preconditions/IsGreaterPrecondition.h"

std::unique_ptr<PreconditionBase> PreconditionFactory::createFromName(const QString &name)
{
    // TODO
    if (name == traits::precondition_name<IsEqualPrecondition<bool>>::value)
    {
        // TODO
        return std::unique_ptr<IsEqualPrecondition<bool>>(new IsEqualPrecondition<bool>());
    }
}
