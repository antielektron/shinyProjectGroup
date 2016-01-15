#include "GameLogic/Factories/ActionFactory.h"

#include "GameLogic/Actions/AssignAttributeAction.h"
#include "GameLogic/Actions/InvertAttributeAction.h"
#include "GameLogic/Actions/IncrementAttributeAction.h"
#include "GameLogic/Actions/DecrementAttributeAction.h"
#include "GameLogic/Actions/MultiplyAttributeAction.h"
#include "GameLogic/Actions/DivideAttributeAction.h"

template <typename ...Args>
struct CreateActionHelper {};

template <typename T, typename ...Args>
struct CreateActionHelper<T, Args...>
{
    static std::unique_ptr<ActionBase> create(const QString &type, GlobalState *state)
    {
        if (type == traits::action_name<T>::value)
        {
            return std::unique_ptr<ActionBase>(new T(state));
        }
        else
        {
            return CreateActionHelper<Args...>::create(type, state);
        }
    }

    static std::unique_ptr<ActionBase> create(const QString &type, GlobalState *state, const QDomElement &domElement)
    {
        if (type == traits::action_name<T>::value)
        {
            return std::unique_ptr<ActionBase>(new T(state, domElement));
        }
        else
        {
            return CreateActionHelper<Args...>::create(type, state, domElement);
        }
    }

    static void getTypes(std::vector<QString> &types)
    {
        types.push_back(QString(traits::action_name<T>::value));
        CreateActionHelper<Args...>::getTypes(types);
    }
};

template <>
struct CreateActionHelper<>
{
    static std::unique_ptr<ActionBase> create(const QString &type, GlobalState *state)
    {
        throw std::runtime_error("Unknown action type: " + type.toStdString());
    }

    static std::unique_ptr<ActionBase> create(const QString &type, GlobalState *state, const QDomElement &domElement)
    {
        throw std::runtime_error("Unknown action type: " + type.toStdString());
    }

    static void getTypes(std::vector<QString> &types)
    {
    }
};

typedef CreateActionHelper<
        AssignAttributeAction<int>,
        AssignAttributeAction<double>,
        AssignAttributeAction<bool>,

        InvertAttributeAction<int>,
        InvertAttributeAction<double>,
        InvertAttributeAction<bool>,

        IncrementAttributeAction<int>,
        IncrementAttributeAction<double>,

        DecrementAttributeAction<int>,
        DecrementAttributeAction<double>,

        MultiplyAttributeAction<int>,
        MultiplyAttributeAction<double>,

        DivideAttributeAction<int>,
        DivideAttributeAction<double>
        > HelperType;

std::unique_ptr<ActionBase> Factory::createActionFromType(GlobalState *state, const QString &type)
{
    return HelperType::create(type, state);
}

std::unique_ptr<ActionBase> Factory::createActionFromDomElement(GlobalState *state, const QDomElement &domElement)
{
    auto type = domElement.attribute("type");

    return HelperType::create(type, state, domElement);
}

std::vector<QString> Factory::getKnownActionTypes()
{
    std::vector<QString> types;

    HelperType::getTypes(types);

    return std::move(types);
}
