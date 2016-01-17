#ifndef UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_EXPRESSION_FACTORY_H
#define UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_EXPRESSION_FACTORY_H

#include <memory>
#include <QString>

template <typename T>
class Expression;

class GlobalState;

namespace Factory
{
    template <typename T>
    std::unique_ptr<Expression<T>> createExpressionFromString(GlobalState *state, const QString &string);
}

#endif // UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_EXPRESSION_FACTORY_H
