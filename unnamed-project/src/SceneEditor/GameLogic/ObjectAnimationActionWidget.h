#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_OBJECT_ANIMATION_ACTION_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_OBJECT_ANIMATION_ACTION_WIDGET_H

#include <QMessageBox>

#include "SceneEditor/GameLogic/ObjectAnimationActionBaseWidget.h"
#include "SceneEditor/SceneEditorGame.h"
#include "GameLogic/Actions/ObjectAnimationAction.h"
#include "GameLogic/Factories/ExpressionFactory.h"
#include "SceneEditor/Factories/InterpolationCreatorDetailsWidgetFactory.h"

template <typename Access>
class ObjectAnimationActionWidget : public ObjectAnimationActionBaseWidget
{
public:
    typedef ObjectAnimationAction<Access> ActionType;

    ObjectAnimationActionWidget(std::shared_ptr<SceneEditorGame> game, ActionType *action, QWidget *parent = nullptr);
    virtual ~ObjectAnimationActionWidget() {}

protected:
    virtual void onApplyClicked() override;
    virtual void onInterpolationTypeChanged() override;
    virtual void onEditInterpolationClicked() override;

private:
    ActionType *m_action;
};

template <typename Access>
ObjectAnimationActionWidget<Access>::ObjectAnimationActionWidget(std::shared_ptr<SceneEditorGame> game, ActionType *action, QWidget *parent) :
        ObjectAnimationActionBaseWidget(game, parent),
        m_action(action)
{
    m_type->setText(m_action->type());

    m_name->setText(m_action->getObjectName());

    m_exprX->setText(m_action->getExprX()->string());
    m_exprY->setText(m_action->getExprY()->string());
    m_exprZ->setText(m_action->getExprZ()->string());

    m_interpolationType->setCurrentText(m_action->getInterpolationCreator()->type());
}

template <typename Access>
void ObjectAnimationActionWidget<Access>::onApplyClicked()
{
    m_action->setObjectName(m_name->text());

    auto exprX = Factory::createExpressionFromString<double>(m_game->getGlobalState(), m_exprX->text());
    m_action->setExprX(std::move(exprX));

    auto exprY = Factory::createExpressionFromString<double>(m_game->getGlobalState(), m_exprY->text());
    m_action->setExprY(std::move(exprY));

    auto exprZ = Factory::createExpressionFromString<double>(m_game->getGlobalState(), m_exprZ->text());
    m_action->setExprZ(std::move(exprZ));

    m_game->notifyEventChanged();
}

template <typename Access>
void ObjectAnimationActionWidget<Access>::onInterpolationTypeChanged()
{
    auto type = m_interpolationType->currentText();
    m_action->setInterpolationCreator(Factory::createInterpolationCreatorFromType(type));
    // NOTE: Silently changing this!
}

template <typename Access>
void ObjectAnimationActionWidget<Access>::onEditInterpolationClicked()
{
    auto widget = Factory::createInterpolationCreatorDetailsWidget(m_game, m_action->getInterpolationCreator(), nullptr);
    widget->show();
}


#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_OBJECT_ANIMATION_ACTION_WIDGET_H
