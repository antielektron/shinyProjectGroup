#ifndef UNNAMED_PROJECT_GAME_LOGIC_EXPRESSIONS_EXPRESSION_H
#define UNNAMED_PROJECT_GAME_LOGIC_EXPRESSIONS_EXPRESSION_H

#include <QString>

template <typename T>
class Expression
{
public:
    typedef T ValueType;

    Expression() {}
    virtual ~Expression() {}

    virtual T evaluate() = 0;
    virtual QString string() = 0;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_EXPRESSIONS_EXPRESSION_H
