#include "GameLogic/Factories/PreconditionFactory.h"

#include "GameLogic/Preconditions/IsEqualPrecondition.h"
#include "GameLogic/Preconditions/IsLessPrecondition.h"
#include "GameLogic/Preconditions/IsGreaterPrecondition.h"

std::unique_ptr<PreconditionBase> PreconditionFactory::createFromName(const QString &name)
{
    if (name == traits::precondition_name<IsEqualPrecondition<int>>::value)
    {
        return std::unique_ptr<PreconditionBase>(new IsEqualPrecondition<int>());
    }
    else if (name == traits::precondition_name<IsEqualPrecondition<double>>::value)
    {
        return std::unique_ptr<PreconditionBase>(new IsEqualPrecondition<double>());
    }
    else if (name == traits::precondition_name<IsEqualPrecondition<bool>>::value)
    {
        return std::unique_ptr<PreconditionBase>(new IsEqualPrecondition<bool>());
    }
    else if (name == traits::precondition_name<IsGreaterPrecondition<int>>::value)
    {
        return std::unique_ptr<PreconditionBase>(new IsGreaterPrecondition<int>());
    }
    else if (name == traits::precondition_name<IsGreaterPrecondition<double>>::value)
    {
        return std::unique_ptr<PreconditionBase>(new IsGreaterPrecondition<double>());
    }
    else if (name == traits::precondition_name<IsLessPrecondition<int>>::value)
    {
        return std::unique_ptr<PreconditionBase>(new IsLessPrecondition<int>());
    }
    else if (name == traits::precondition_name<IsLessPrecondition<double>>::value)
    {
        return std::unique_ptr<PreconditionBase>(new IsLessPrecondition<double>());
    }
    else
    {
        throw std::runtime_error("Unknown precondition type: " + name.toStdString());
    }
}
