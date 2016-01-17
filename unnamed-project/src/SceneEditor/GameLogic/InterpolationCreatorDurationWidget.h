#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_INTERPOLATION_CREATOR_DURATION_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_INTERPOLATION_CREATOR_DURATION_WIDGET_H

#include "SceneEditor/GameLogic/InterpolationCreatorDurationBaseWidget.h"
#include "GameLogic/InterpolationCreator/InterpolationCreatorDuration.h"
#include "SceneEditor/SceneEditorGame.h"

template <typename Interpolation>
class InterpolationCreatorDurationWidget : public InterpolationCreatorDurationBaseWidget
{
public:
    InterpolationCreatorDurationWidget(std::shared_ptr<SceneEditorGame> game, InterpolationCreatorDuration<Interpolation> *interpolationCreator, QWidget *parent = nullptr);
    virtual ~InterpolationCreatorDurationWidget() {}

protected:
    virtual void onApplyClicked() override;

    InterpolationCreatorDuration<Interpolation> *m_interpolationCreator;
};

template <typename Interpolation>
InterpolationCreatorDurationWidget<Interpolation>::InterpolationCreatorDurationWidget(std::shared_ptr<SceneEditorGame> game, InterpolationCreatorDuration<Interpolation> *interpolationCreator, QWidget *parent) :
        InterpolationCreatorDurationBaseWidget(game, parent),
        m_interpolationCreator(interpolationCreator)
{
    m_type->setText(m_interpolationCreator->type());
    m_duration->setText(m_interpolationCreator->getDuration()->string());
}

template <typename Interpolation>
void InterpolationCreatorDurationWidget<Interpolation>::onApplyClicked()
{
    // NOTE: Silently changing this value!

    auto duration = Factory::createExpressionFromString<double>(m_game->getGlobalState(), m_duration->text());
    m_interpolationCreator->setDuration(std::move(duration));

    close();
}

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_INTERPOLATION_CREATOR_DURATION_WIDGET_H
