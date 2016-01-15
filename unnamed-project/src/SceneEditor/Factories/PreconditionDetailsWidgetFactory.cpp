#include "SceneEditor/Factories/PreconditionDetailsWidgetFactory.h"
#include "SceneEditor/SceneEditorGame.h"
#include "SceneEditor/GameLogic/BinaryPreconditionWidget.h"

#include "GameLogic/Preconditions/IsEqualPrecondition.h"
#include "GameLogic/Preconditions/IsLessPrecondition.h"
#include "GameLogic/Preconditions/IsGreaterPrecondition.h"

#include <cassert>

// for any combination
template <typename Precondition, typename Widget>
struct PreconditionMap
{
    typedef Precondition PreconditionType;
    typedef Widget WidgetType;
};

// for our binary preconditions..
template <typename Precondition>
struct BinaryPreconditionMap
{
    typedef Precondition PreconditionType;
    typedef BinaryPreconditionWidget<typename Precondition::ExpressionType::ValueType> WidgetType;
};


template <typename ...Args>
struct CreatePreconditionDetailsWidgetHelper {};

template <typename T, typename ...Args>
struct CreatePreconditionDetailsWidgetHelper<T, Args...>
{
    static QWidget *create(const QString &type, std::shared_ptr<SceneEditorGame> game, PreconditionBase *precondition, QWidget *parent)
    {
        // TODO maybe try to dynamic cast instead of name comparision?
        if (type == traits::precondition_name<typename T::PreconditionType>::value)
        {
            auto c = dynamic_cast<typename T::PreconditionType *>(precondition);
            assert(c != nullptr);
            return new typename T::WidgetType(game, c, parent);
        }
        else
        {
            return CreatePreconditionDetailsWidgetHelper<Args...>::create(type, game, precondition, parent);
        }
    }
};

template <>
struct CreatePreconditionDetailsWidgetHelper<>
{
    static QWidget *create(const QString &type, std::shared_ptr<SceneEditorGame> game, PreconditionBase *precondition, QWidget *parent)
    {
        throw std::runtime_error("Unknown precondition type: " + type.toStdString());
    }
};

typedef CreatePreconditionDetailsWidgetHelper<
        BinaryPreconditionMap<IsEqualPrecondition<int>>,
        BinaryPreconditionMap<IsEqualPrecondition<double>>,
        BinaryPreconditionMap<IsEqualPrecondition<bool>>,
        BinaryPreconditionMap<IsGreaterPrecondition<int>>,
        BinaryPreconditionMap<IsGreaterPrecondition<double>>,
        BinaryPreconditionMap<IsLessPrecondition<int>>,
        BinaryPreconditionMap<IsLessPrecondition<double>>
> HelperType;

QWidget *Factory::createPreconditionDetailsWidget(std::shared_ptr<SceneEditorGame> game, PreconditionBase *precondition, QWidget *parent)
{
    // assume we only have
    return HelperType::create(precondition->type(), game, precondition, parent);
}
