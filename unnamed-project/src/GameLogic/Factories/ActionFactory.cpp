#include "GameLogic/Factories/ActionFactory.h"

using namespace Factory;

// TODO

std::unique_ptr<ActionBase> Factory::createActionFromType(const QString &type)
{
    throw std::runtime_error("Not implemented yet");
}

std::unique_ptr<ActionBase> Factory::createActionFromDomElement(const QDomElement &domElement)
{
    throw std::runtime_error("Not implemented yet");
}

std::vector<QString> Factory::getKnownActionTypes()
{
    std::vector<QString> types;

    // TODO

    return std::move(types);
}
