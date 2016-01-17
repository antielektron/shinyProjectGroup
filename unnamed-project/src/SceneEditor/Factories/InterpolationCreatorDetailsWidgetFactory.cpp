#include "SceneEditor/Factories/InterpolationCreatorDetailsWidgetFactory.h"

#include "GameLogic/InterpolationCreator/InterpolationCreatorDuration.h"
#include "SceneEditor/GameLogic/InterpolationCreatorDurationWidget.h"

#include <cassert>

// for any combination
template <typename InterpolationCreator, typename Widget>
struct WidgetMap
{
    typedef InterpolationCreator InterpolationCreatorType;
    typedef Widget WidgetType;
};

// TODO apply this to other widget factories!
// for our binary preconditions..
template <typename Interpolation>
struct InterpolationDurationMap
{
    typedef InterpolationCreatorDuration<Interpolation> InterpolationCreatorType;
    typedef InterpolationCreatorDurationWidget<Interpolation> WidgetType;
};


template <typename ...Args>
struct CreateInterpolationCreatorDetailsWidgetHelper {};

template <typename T, typename ...Args>
struct CreateInterpolationCreatorDetailsWidgetHelper<T, Args...>
{
    static QWidget *create(const QString &type, std::shared_ptr<SceneEditorGame> game, InterpolationCreatorBase *interpolationCreator, QWidget *parent)
    {
        // TODO maybe try to dynamic cast instead of name comparision?
        if (type == traits::interpolation_name<typename T::InterpolationCreatorType>::value())
        {
            auto c = dynamic_cast<typename T::InterpolationCreatorType *>(interpolationCreator);
            assert(c != nullptr);
            return new typename T::WidgetType(game, c, parent);
        }
        else
        {
            return CreateInterpolationCreatorDetailsWidgetHelper<Args...>::create(type, game, interpolationCreator, parent);
        }
    }
};

template <>
struct CreateInterpolationCreatorDetailsWidgetHelper<>
{
    static QWidget *create(const QString &type, std::shared_ptr<SceneEditorGame> game, InterpolationCreatorBase *interpolationCreator, QWidget *parent)
    {
        throw std::runtime_error("Unknown interpolation type: " + type.toStdString());
    }
};

typedef CreateInterpolationCreatorDetailsWidgetHelper<
        InterpolationDurationMap<InterpolationLinear>,
        InterpolationDurationMap<InterpolationForgotName>,
        InterpolationDurationMap<InterpolationSin>
> HelperType;


QWidget *Factory::createInterpolationCreatorDetailsWidget(std::shared_ptr<SceneEditorGame> game, InterpolationCreatorBase *interpolationCreator, QWidget *parent)
{
    return HelperType::create(interpolationCreator->type(), game, interpolationCreator, parent);
}
