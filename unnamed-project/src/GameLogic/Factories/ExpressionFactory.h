#ifndef UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_EXPRESSION_FACTORY_H
#define UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_EXPRESSION_FACTORY_H

#include <memory>
#include <QString>

#include "GameLogic/Expressions/ExpressionVariable.h"
#include "GameLogic/Expressions/ExpressionConstant.h"

namespace Factory
{
    template <typename T>
    std::unique_ptr<Expression<T>> createExpressionFromString(GlobalState *state, const QString &string);

    template <>
    std::unique_ptr<Expression<int>> createExpressionFromString(GlobalState *state, const QString &string)
    {
        // TODO advanced syntax analysis

        if (string.startsWith('[') && string.endsWith(']'))
        {
            QString key = string.mid(1, string.length()-2);
            return std::unique_ptr<Expression<int>>(new ExpressionVariable<int>(state, key));
        }
        else
        {
            bool ok;
            auto result = string.toInt(&ok);

            if (ok)
            {
                return std::unique_ptr<Expression<int>>(new ExpressionConstant<int>(result));
            }
            else
            {
                throw std::runtime_error("This is not an integer value: " + string.toStdString());
            }
        }
    }

    template <>
    std::unique_ptr<Expression<double>> createExpressionFromString(GlobalState *state, const QString &string)
    {
        // TODO advanced syntax analysis

        if (string.startsWith('[') && string.endsWith(']'))
        {
            QString key = string.mid(1, string.length()-2);
            return std::unique_ptr<Expression<double>>(new ExpressionVariable<double>(state, key));
        }
        else
        {
            bool ok;
            auto result = string.toDouble(&ok);

            if (ok)
            {
                return std::unique_ptr<Expression<double>>(new ExpressionConstant<double>(result));
            }
            else
            {
                throw std::runtime_error("This is not a double value: " + string.toStdString());
            }
        }
    }

    template <>
    std::unique_ptr<Expression<bool>> createExpressionFromString(GlobalState *state, const QString &string)
    {
        // TODO advanced syntax analysis

        if (string.startsWith('[') && string.endsWith(']'))
        {
            QString key = string.mid(1, string.length()-2);
            return std::unique_ptr<Expression<bool>>(new ExpressionVariable<bool>(state, key));
        }
        else if (string == "true")
        {
            return std::unique_ptr<Expression<bool>>(new ExpressionConstant<bool>(true));
        }
        else if (string == "false")
        {
            return std::unique_ptr<Expression<bool>>(new ExpressionConstant<bool>(false));
        }
        else
        {
            bool ok;
            int result = string.toInt(&ok);

            if (ok)
            {
                return std::unique_ptr<Expression<bool>>(new ExpressionConstant<bool>(result != 0));
            }
            else
            {
                throw std::runtime_error("This is not a boolean value: " + string.toStdString());
            }
        }
    }
}

#endif // UNNAMED_PROJECT_GAME_LOGIC_FACTORIES_EXPRESSION_FACTORY_H
