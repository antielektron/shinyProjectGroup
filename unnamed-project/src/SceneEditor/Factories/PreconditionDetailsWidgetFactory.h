#ifndef UNNAMED_PROJECT_SCENE_EDITOR_FACTORIES_PRECONDITION_DETAILS_WIDGET_FACTORY_H
#define UNNAMED_PROJECT_SCENE_EDITOR_FACTORIES_PRECONDITION_DETAILS_WIDGET_FACTORY_H

#include <memory>
#include <QWidget>

class SceneEditorGame;
class PreconditionBase;

namespace Factory
{
    QWidget *createPreconditionDetailsWidget(std::shared_ptr<SceneEditorGame> game, PreconditionBase *precondition, QWidget *parent = nullptr);
}

#endif // UNNAMED_PROJECT_SCENE_EDITOR_FACTORIES_PRECONDITION_DETAILS_WIDGET_FACTORY_H
