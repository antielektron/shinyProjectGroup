#ifndef UNNAMED_PROJECT_GAME_LOGIC_EXPRESSIONS_EXPRESSION_VARIABLE_H
#define UNNAMED_PROJECT_GAME_LOGIC_EXPRESSIONS_EXPRESSION_VARIABLE_H

#include "GameLogic/Expressions/Expression.h"
#include "GameLogic/GlobalState.h"

template <typename T>
class ExpressionVariable : public Expression<T>
{
public:
    ExpressionVariable(GlobalState *state, const QString& key) : m_globalState(state), m_key(key) {}
    virtual ~ExpressionVariable() {}

    virtual T evaluate() override
    {
        return m_globalState->getValue(m_key).template value<T>();
    }

    virtual QString string() override
    {
        return QString("[") + m_key + "]";
    }

private:
    GlobalState *m_globalState;
    QString m_key;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_EXPRESSIONS_EXPRESSION_BASE_H
