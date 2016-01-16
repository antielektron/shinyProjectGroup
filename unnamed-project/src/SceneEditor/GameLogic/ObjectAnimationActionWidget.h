#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_OBJECT_ANIMATION_ACTION_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_OBJECT_ANIMATION_ACTION_WIDGET_H

#include <QMessageBox>

#include "SceneEditor/GameLogic/ObjectAnimationActionBaseWidget.h"
#include "SceneEditor/SceneEditorGame.h"
#include "GameLogic/Actions/ObjectAnimationAction.h"
#include "GameLogic/Factories/ExpressionFactory.h"

template <typename Access>
class ObjectAnimationActionWidget : public ObjectAnimationActionBaseWidget
{
public:
    typedef ObjectAnimationAction<Access> ActionType;

    ObjectAnimationActionWidget(std::shared_ptr<SceneEditorGame> game, ActionType *action, QWidget *parent = nullptr);
    virtual ~ObjectAnimationActionWidget() {}

protected:
    virtual void onApplyClicked() override;

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

    m_duration->setText(m_action->getDuration()->string());
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

    auto duration = Factory::createExpressionFromString<double>(m_game->getGlobalState(), m_duration->text());
    m_action->setDuration(std::move(duration));

    m_game->notifyEventChanged();
}

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_OBJECT_ANIMATION_ACTION_WIDGET_H
