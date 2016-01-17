#include "SceneEditor/Factories/ActionDetailsWidgetFactory.h"
#include "SceneEditor/SceneEditorGame.h"

#include "SceneEditor/GameLogic/InvertAttributeActionWidget.h"
#include "SceneEditor/GameLogic/ArithmeticActionWidget.h"

#include "SceneEditor/GameLogic/ObjectAnimationActionWidget.h"
#include "SceneEditor/GameLogic/SceneAnimationActionWidget.h"

#include "GameLogic/Actions/InvertAttributeAction.h"
#include "GameLogic/Actions/AssignAttributeAction.h"
#include "GameLogic/Actions/IncrementAttributeAction.h"
#include "GameLogic/Actions/DecrementAttributeAction.h"
#include "GameLogic/Actions/MultiplyAttributeAction.h"
#include "GameLogic/Actions/DivideAttributeAction.h"

#include <cassert>

// for any combination
template <typename Action, typename Widget>
struct ActionMap
{
    typedef Action ActionType;
    typedef Widget WidgetType;
};

// for our arithmetic actions..
template <typename Action>
struct ArithmeticActionMap
{
    typedef Action ActionType;
    typedef ArithmeticActionWidget<typename Action::ExpressionType::ValueType> WidgetType;
};

// for our invert actions..
template <typename Action>
struct InvertAttributeActionMap
{
    typedef Action ActionType;
    typedef InvertAttributeActionWidget<typename Action::ValueType> WidgetType;
};

// for our object animation actions..
template <typename Access>
struct ObjectAnimationActionMap
{
    typedef ObjectAnimationAction<Access> ActionType;
    typedef ObjectAnimationActionWidget<Access> WidgetType;
};

// for our scene animation actions..
template <typename Access>
struct SceneAnimationActionMap
{
    typedef SceneAnimationAction<Access> ActionType;
    typedef SceneAnimationActionWidget<Access> WidgetType;
};

template <typename ...Args>
struct CreateActionDetailsWidgetHelper {};

template <typename T, typename ...Args>
struct CreateActionDetailsWidgetHelper<T, Args...>
{
    static QWidget *create(const QString &type, std::shared_ptr<SceneEditorGame> game, ActionBase *action, QWidget *parent)
    {
        // TODO maybe try to dynamic cast instead of name comparision?
        if (type == traits::action_name<typename T::ActionType>::value())
        {
            auto c = dynamic_cast<typename T::ActionType *>(action);
            assert(c != nullptr);
            return new typename T::WidgetType(game, c, parent);
        }
        else
        {
            return CreateActionDetailsWidgetHelper<Args...>::create(type, game, action, parent);
        }
    }
};

template <>
struct CreateActionDetailsWidgetHelper<>
{
    static QWidget *create(const QString &type, std::shared_ptr<SceneEditorGame> game, ActionBase *action, QWidget *parent)
    {
        throw std::runtime_error("Unknown action type: " + type.toStdString());
    }
};

typedef CreateActionDetailsWidgetHelper<
        InvertAttributeActionMap<InvertAttributeAction<int>>,
        InvertAttributeActionMap<InvertAttributeAction<double>>,
        InvertAttributeActionMap<InvertAttributeAction<bool>>,

        ArithmeticActionMap<AssignAttributeAction<int>>,
        ArithmeticActionMap<AssignAttributeAction<double>>,
        ArithmeticActionMap<AssignAttributeAction<bool>>,

        ArithmeticActionMap<IncrementAttributeAction<int>>,
        ArithmeticActionMap<IncrementAttributeAction<double>>,

        ArithmeticActionMap<DecrementAttributeAction<int>>,
        ArithmeticActionMap<DecrementAttributeAction<double>>,

        ArithmeticActionMap<MultiplyAttributeAction<int>>,
        ArithmeticActionMap<MultiplyAttributeAction<double>>,

        ArithmeticActionMap<DivideAttributeAction<int>>,
        ArithmeticActionMap<DivideAttributeAction<double>>,

        ObjectAnimationActionMap<AccessObjectPosition>,
        ObjectAnimationActionMap<AccessObjectRotation>,
        ObjectAnimationActionMap<AccessObjectAmbientColor>,
        ObjectAnimationActionMap<AccessObjectDiffuseColor>,
        ObjectAnimationActionMap<AccessObjectSpecularColor>,

        SceneAnimationActionMap<AccessLightDirection>,
        SceneAnimationActionMap<AccessLightColor>
> HelperType;

QWidget *Factory::createActionDetailsWidget(std::shared_ptr<SceneEditorGame> game, ActionBase *action, QWidget *parent)
{
    // assume we only have
    return HelperType::create(action->type(), game, action, parent);
}
