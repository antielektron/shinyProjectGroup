#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_EXPRESSIONS_EXPRESSION_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_EXPRESSIONS_EXPRESSION_H

#include <QString>

template <typename T>
class Expression
{
public:
    Expression() {}
    virtual ~Expression() {}

    virtual T evaluate() = 0;
    virtual QString toQString() = 0;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_EXPRESSIONS_EXPRESSION_BASE_H
