#include "GameLogic/Factories/PreconditionFactory.h"

#include "GameLogic/Preconditions/IsEqualPrecondition.h"
#include "GameLogic/Preconditions/IsLessPrecondition.h"
#include "GameLogic/Preconditions/IsGreaterPrecondition.h"

template <typename ...Args>
struct CreatePreconditionHelper {};

template <typename T, typename ...Args>
struct CreatePreconditionHelper<T, Args...>
{
    static std::unique_ptr<PreconditionBase> create(const QString &type)
    {
        if (type == traits::precondition_name<T>::value)
        {
            return std::unique_ptr<PreconditionBase>(new T());
        }
        else
        {
            return CreatePreconditionHelper<Args...>::create(type);
        }
    }

    static std::unique_ptr<PreconditionBase> create(const QString &type, GlobalState *state, const QDomElement &domElement)
    {
        if (type == traits::precondition_name<T>::value)
        {
            return std::unique_ptr<PreconditionBase>(new T(state, domElement));
        }
        else
        {
            return CreatePreconditionHelper<Args...>::create(type, state, domElement);
        }
    }

    static void getTypes(std::vector<QString> &types)
    {
        types.push_back(QString(traits::precondition_name<T>::value));
        CreatePreconditionHelper<Args...>::getTypes(types);
    }
};

template <>
struct CreatePreconditionHelper<>
{
    static std::unique_ptr<PreconditionBase> create(const QString &type)
    {
        throw std::runtime_error("Unknown precondition type: " + type.toStdString());
    }

    static std::unique_ptr<PreconditionBase> create(const QString &type, GlobalState *state, const QDomElement &domElement)
    {
        throw std::runtime_error("Unknown precondition type: " + type.toStdString());
    }

    static void getTypes(std::vector<QString> &types)
    {
    }
};

typedef CreatePreconditionHelper<
        IsEqualPrecondition<int>,
        IsEqualPrecondition<double>,
        IsEqualPrecondition<bool>,

        IsGreaterPrecondition<int>,
        IsGreaterPrecondition<double>,

        IsLessPrecondition<int>,
        IsLessPrecondition<double>
> HelperType;

std::unique_ptr<PreconditionBase> Factory::createPreconditionFromType(const QString &type)
{
    return HelperType::create(type);
}

std::unique_ptr<PreconditionBase> Factory::createPreconditionFromDomElement(GlobalState *state, const QDomElement &domElement)
{
    auto type = domElement.attribute("type");

    return HelperType::create(type, state, domElement);
}

std::vector<QString> Factory::getKnownPreconditionTypes()
{
    std::vector<QString> types;

    HelperType::getTypes(types);

    return std::move(types);
}
