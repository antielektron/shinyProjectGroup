#ifndef UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_INTERPOLATION_CREATOR_FACTORY_H
#define UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_INTERPOLATION_CREATOR_FACTORY_H

#include <memory>
#include <vector>

#include <QString>
#include <QDomElement>

class InterpolationCreatorBase;
class GlobalState;

namespace Factory
{
    std::unique_ptr<InterpolationCreatorBase> createInterpolationCreatorFromType(const QString &type);
    std::unique_ptr<InterpolationCreatorBase> createInterpolationCreatorFromDomElement(GlobalState *state, const QDomElement &domElement);

    std::vector<QString> getKnownInterpolationCreatorTypes();
}

#endif // UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_INTERPOLATION_CREATOR_FACTORY_H
