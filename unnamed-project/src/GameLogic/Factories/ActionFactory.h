#ifndef UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_ACTION_FACTORY_H
#define UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_ACTION_FACTORY_H

#include <memory>
#include <QString>
#include <QDomElement>

class ActionBase;

namespace Factory
{
    std::unique_ptr<ActionBase> createActionFromType(const QString &type);
    std::unique_ptr<ActionBase> createActionFromDomElement(const QDomElement &domElement);

    std::vector<QString> getKnownActionTypes();
}

#endif // UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_ACTION_FACTORY_H
