#ifndef UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_ACTION_FACTORY_H
#define UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_ACTION_FACTORY_H

#include <memory>
#include <QString>
#include <QDomElement>

class ActionBase;
class GlobalState;

namespace Factory
{
    std::unique_ptr<ActionBase> createActionFromType(GlobalState *state, const QString &type);
    std::unique_ptr<ActionBase> createActionFromDomElement(GlobalState *state, const QDomElement &domElement);

    std::vector<QString> getKnownActionTypes();
}

#endif // UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_ACTION_FACTORY_H
