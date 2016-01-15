#ifndef UNNAMED_PROJECT_SCENE_EDITOR_FACTORIES_ACTION_DETAILS_WIDGET_FACTORY_H
#define UNNAMED_PROJECT_SCENE_EDITOR_FACTORIES_ACTION_DETAILS_WIDGET_FACTORY_H

#include <memory>
#include <QWidget>

class SceneEditorGame;
class ActionBase;

namespace Factory
{
    QWidget *createActionDetailsWidget(std::shared_ptr<SceneEditorGame> game, ActionBase *precondition, QWidget *parent = nullptr);
}

#endif // UNNAMED_PROJECT_SCENE_EDITOR_FACTORIES_ACTION_DETAILS_WIDGET_FACTORY_H
