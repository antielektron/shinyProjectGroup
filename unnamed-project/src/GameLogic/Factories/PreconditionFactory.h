#ifndef UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_PRECONDITION_FACTORY_H
#define UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_PRECONDITION_FACTORY_H

#include <memory>
#include <QString>

class PreconditionBase;

class PreconditionFactory
{
public:
    static std::unique_ptr<PreconditionBase> createFromName(const QString &name);
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_PRECONDITION_FACTORY_H
