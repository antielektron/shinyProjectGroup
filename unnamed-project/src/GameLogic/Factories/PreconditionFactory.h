#ifndef UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_PRECONDITION_FACTORY_H
#define UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_PRECONDITION_FACTORY_H

#include <memory>
#include <vector>
#include <QString>
#include <QDomElement>

class PreconditionBase;

namespace Factory
{
    std::unique_ptr<PreconditionBase> createPreconditionFromType(const QString &type);
    std::unique_ptr<PreconditionBase> createPreconditionFromDomElement(const QDomElement &domElement);

    std::vector<QString> getKnownPreconditionTypes();
}

#endif // UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_PRECONDITION_FACTORY_H
