#ifndef UNNAMED_PROJECT_SCENE_EDITOR_FACTORIES_INTERPOLATION_CREATOR_DETAILS_WIDGET_FACTORY_H
#define UNNAMED_PROJECT_SCENE_EDITOR_FACTORIES_INTERPOLATION_CREATOR_DETAILS_WIDGET_FACTORY_H

#include <memory>
#include <QWidget>

class SceneEditorGame;
class InterpolationCreatorBase;

namespace Factory
{
    QWidget *createInterpolationCreatorDetailsWidget(std::shared_ptr<SceneEditorGame> game, InterpolationCreatorBase *interpolationCreator, QWidget *parent = nullptr);
}

#endif // UNNAMED_PROJECT_SCENE_EDITOR_FACTORIES_INTERPOLATION_CREATOR_DETAILS_WIDGET_FACTORY_H
