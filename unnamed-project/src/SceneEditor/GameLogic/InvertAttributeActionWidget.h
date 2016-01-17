#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_INVERT_ATTRIBUTE_ACTION_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_INVERT_ATTRIBUTE_ACTION_WIDGET_H

#include "SceneEditor/GameLogic/UnaryBaseWidget.h"
#include "SceneEditor/SceneEditorGame.h"
#include "GameLogic/Actions/InvertAttributeAction.h"

template <typename T>
class InvertAttributeActionWidget : public UnaryBaseWidget
{
public:
    typedef InvertAttributeAction<T> ActionType;

    InvertAttributeActionWidget(std::shared_ptr<SceneEditorGame> game, ActionType *action, QWidget *parent = nullptr);
    virtual ~InvertAttributeActionWidget();

protected:
    virtual void onApplyClicked() override;

private:
    ActionType *m_action;
};


template <typename T>
InvertAttributeActionWidget<T>::InvertAttributeActionWidget(std::shared_ptr<SceneEditorGame> game, ActionType *action, QWidget *parent) :
        UnaryBaseWidget(game, parent),
        m_action(action)
{
    // Set UI values
    m_type->setText(m_action->type());
    m_key->setText(m_action->getKey());
}

template <typename T>
InvertAttributeActionWidget<T>::~InvertAttributeActionWidget()
{}

template <typename T>
void InvertAttributeActionWidget<T>::onApplyClicked()
{
    m_action->setKey(m_key->text());

    m_game->notifyEventChanged();
}


#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_INVERT_ATTRIBUTE_ACTION_WIDGET_H

