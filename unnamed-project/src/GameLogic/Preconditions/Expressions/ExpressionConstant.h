#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_EXPRESSIONS_EXPRESSION_CONSTANT_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_EXPRESSIONS_EXPRESSION_CONSTANT_H

#include <string>
#include "GameLogic/Preconditions/Expressions/Expression.h"

template <typename T>
class ExpressionConstant : public Expression<T>
{
public:
    ExpressionConstant() : m_value{T()} {}
    ExpressionConstant(const T& value) : m_value(value) {}
    virtual ~ExpressionConstant() {}

    virtual T evaluate() override { return m_value; }

    virtual QString string() override { return QString::fromStdString(std::to_string(m_value)); }

private:
    T m_value;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_EXPRESSIONS_EXPRESSION_BASE_H
