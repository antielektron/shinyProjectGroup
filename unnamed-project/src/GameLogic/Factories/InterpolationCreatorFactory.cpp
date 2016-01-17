#include "GameLogic/Factories/InterpolationCreatorFactory.h"

#include "GameLogic/InterpolationCreator/InterpolationCreatorDuration.h"

template <typename ...Args>
struct CreateInterpolationCreatorHelper {};

template <typename T, typename ...Args>
struct CreateInterpolationCreatorHelper<T, Args...>
{
    static std::unique_ptr<InterpolationCreatorBase> create(const QString &type)
    {
        if (type == traits::interpolation_name<T>::value())
        {
            return std::unique_ptr<InterpolationCreatorBase>(new T());
        }
        else
        {
            return CreateInterpolationCreatorHelper<Args...>::create(type);
        }
    }

    static std::unique_ptr<InterpolationCreatorBase> create(const QString &type, GlobalState *state, const QDomElement &domElement)
    {
        if (type == traits::interpolation_name<T>::value())
        {
            return std::unique_ptr<InterpolationCreatorBase>(new T(state, domElement));
        }
        else
        {
            return CreateInterpolationCreatorHelper<Args...>::create(type, state, domElement);
        }
    }

    static void getTypes(std::vector<QString> &types)
    {
        types.push_back(QString(traits::interpolation_name<T>::value()));
        CreateInterpolationCreatorHelper<Args...>::getTypes(types);
    }
};

template <>
struct CreateInterpolationCreatorHelper<>
{
    static std::unique_ptr<InterpolationCreatorBase> create(const QString &type)
    {
        throw std::runtime_error("Unknown interpolation type: " + type.toStdString());
    }

    static std::unique_ptr<InterpolationCreatorBase> create(const QString &type, GlobalState *state, const QDomElement &domElement)
    {
        throw std::runtime_error("Unknown interpolation type: " + type.toStdString());
    }

    static void getTypes(std::vector<QString> &types)
    {
    }
};

typedef CreateInterpolationCreatorHelper<
        InterpolationCreatorDuration<InterpolationLinear>,
        InterpolationCreatorDuration<InterpolationForgotName>,
        InterpolationCreatorDuration<InterpolationSin>
> HelperType;


std::unique_ptr<InterpolationCreatorBase> Factory::createInterpolationCreatorFromType(const QString &type)
{
    return HelperType::create(type);
}

std::unique_ptr<InterpolationCreatorBase> Factory::createInterpolationCreatorFromDomElement(GlobalState *state, const QDomElement &domElement)
{
    auto type = domElement.attribute("type");

    return HelperType::create(type, state, domElement);
}

std::vector<QString> Factory::getKnownInterpolationCreatorTypes()
{
    std::vector<QString> types;

    HelperType::getTypes(types);

    return std::move(types);
}
