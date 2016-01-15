#ifndef UNNAMED_PROJECT_GAME_LOGIC_EXPRESSIONS_EXPRESSION_EXTERN_H
#define UNNAMED_PROJECT_GAME_LOGIC_EXPRESSIONS_EXPRESSION_EXTERN_H

#include "GameLogic/Expressions/Expression.h"
#include "GameLogic/GlobalState.h"

template <typename T>
class ExpressionExtern : public Expression<T>
{
public:
    ExpressionExtern(GlobalState *state, const QString& key) : m_globalState(state), m_key(key) {}
    virtual ~ExpressionExtern() {}

    virtual T evaluate() override
    {
        // TODO !!
        throw std::runtime_error("No");
    }

    virtual QString string() override
    {
        return QString("{") + m_key + "}";
    }

private:
    GlobalState *m_globalState;
    QString m_key;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_EXPRESSIONS_EXPRESSION_EXTERN_H

