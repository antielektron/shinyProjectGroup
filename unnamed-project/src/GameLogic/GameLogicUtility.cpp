#include "GameLogic/GameLogicUtility.h"
#include "GameLogic/Preconditions/ConjunctionPrecondition.h"
#include "GameLogic/Preconditions/DisjunctionPrecondition.h"
#include "GameLogic/Preconditions/NegationPrecondition.h"
#include "GameLogic/Preconditions/IsEqualPrecondition.h"
#include "GameLogic/Preconditions/IsGreaterPrecondition.h"
#include "GameLogic/GlobalState.h"

#include <cassert>
#include <stdexcept>

// DEBUG
#include <iostream>

//------------------------------------------------------------------------------
std::string gameLogicUtility::getSubstringByDelimiters(const std::string &src,
                                                       char leftDelimiter,
                                                       char rightDelimiter)
{
    size_t indexLeftBracket = 0;
    size_t indexRightBracket = 0;

    int depth = 0;

    bool findFirstBracket = false;
    bool findLastBracket = false;

    for (size_t i = 0; i < src.length(); i++)
    {
        if (src[i] == leftDelimiter)
        {
            findFirstBracket = true;
            if (depth == 0)
            {
                indexLeftBracket = i;
            }
            depth++;
        }
        else if(src[i] == rightDelimiter && findFirstBracket)
        {
            depth--;
            if (depth == 0)
            {
                indexRightBracket = i;
                findLastBracket = true;
                break;
            }
        }
    }

    if (findFirstBracket && findLastBracket)
    {
        return src.substr(indexLeftBracket + 1,
                          indexRightBracket - indexLeftBracket - 1);
    }

    return ""; // maybe throw an exeption instead

}

//------------------------------------------------------------------------------
std::pair<size_t, char> gameLogicUtility::findOperator(const std::string &src)
{
    size_t opIndex = std::string::npos;
    char op = 0;

    int depth = 0;

    for (size_t i = 0; i < src.length(); i++)
    {
        if (src[i] == BRACKET_LEFT)
        {
            depth++;
        }
        else if (src[i] == BRACKET_RIGHT)
        {
            depth--;
        }
        if (depth == 0)
        {
            if (src[i] == OP_AND)
            {
                op = OP_AND;
                opIndex = i;
                break;
            }
            if (src[i] == OP_OR)
            {
                op = OP_OR;
                opIndex = i;
                break;
            }
            if (src[i] == OP_EQUALS)
            {
                op = OP_EQUALS;
                opIndex = i;
                break;
            }
            if (src[i] == OP_GREATER)
            {
                op = OP_GREATER;
                opIndex = i;
                break;
            }
            if (src[i] == OP_LESS)
            {
                op = OP_LESS;
                opIndex = i;
                break;
            }
            if (src[i] == OP_NOT)
            {
                op = OP_NOT;
                opIndex = i;
                break;
            }
        }
    }

    return std::make_pair(opIndex, op);
}

//------------------------------------------------------------------------------
std::unique_ptr<PreconditionBase> gameLogicUtility::stringToPrecondition(
        GlobalState *state,
        const std::string &input)
{
    // at first, check for operators:

    auto opPair = findOperator(input);

    size_t opIndex = opPair.first;
    char op = opPair.second;

    if (opIndex == std::string::npos)
    {
        // this is a problem
        throw std::runtime_error("missing operator");
    }

    std::string leftSubstring = input.substr(0, opIndex);
    std::string rightSubstring = input.substr(opIndex + 1,
                                              input.length() - opIndex - 1);

    // DEBUG
    std::cout << "source String: "
              << input << std::endl;

    if (op == OP_EQUALS || op == OP_GREATER || op == OP_LESS)
    {
        // in this case we have to determine the type
        // and values from the substrings

        std::string leftName = leftSubstring;
        //remove whitespaces:
        leftName.erase(std::remove (leftName.begin(), leftName.end(), ' '), leftName.end());

        std::string rightName = rightSubstring;
        // remove whitespaces:
        rightName.erase(std::remove (rightName.begin(), rightName.end(), ' '), rightName.end());

        // check if vars exists
        if (!state->existValue(QString::fromStdString(leftName)))
        {
            throw std::runtime_error("unknown variable: " + leftName);
        }
        if (!state->existValue(QString::fromStdString(rightName)))
        {
            throw std::runtime_error("unknown variable: " + rightName);
        }

        AttributeDatatype leftType = state->getType(QString::fromStdString(leftName));

        AttributeDatatype rightType = state->getType(QString::fromStdString(rightName));

        // DEBUG
        std::cout << "leftType: "       << typeToQString.at(leftType).toStdString()
                  << "\nrightType: "    << typeToQString.at(rightType).toStdString()
                  << "\nleftName: "     << leftName
                  << "\nrightName: "    << rightName
                  << std::endl;

        if(!(leftType == rightType))
        {
            throw std::runtime_error("attributes " + leftName + " and "
                                     + rightName + "have different types");
        }

        if (op == OP_EQUALS)
        {
            if (leftType == AttributeDatatype::Int)
            {
                return std::unique_ptr<PreconditionBase>(
                           new IsEqualPrecondition<int>(
                               state,
                               QString::fromStdString(leftName),
                               QString::fromStdString(rightName)));
            }
            else if (leftType == AttributeDatatype::Bool)
            {
                return std::unique_ptr<PreconditionBase>(
                           new IsEqualPrecondition<bool>(
                               state,
                               QString::fromStdString(leftName),
                               QString::fromStdString(rightName)));
            }
        }
        if (op == OP_GREATER)
        {
            if (leftType == AttributeDatatype::Int)
            {
                return std::unique_ptr<PreconditionBase>(
                           new IsGreaterPrecondition<int>(
                               state,
                               QString::fromStdString(leftName),
                               QString::fromStdString(rightName)));
            }
            else if (leftType == AttributeDatatype::Float)
            {
                return std::unique_ptr<PreconditionBase>(
                           new IsGreaterPrecondition<float>(
                               state,
                               QString::fromStdString(leftName),
                               QString::fromStdString(rightName)));
            }
        }
        if (op == OP_LESS)
        {
            // we will use IsGreater condition,
            // but switch left and right operator
            if (leftType == AttributeDatatype::Int)
            {
                return std::unique_ptr<PreconditionBase>(
                           new IsGreaterPrecondition<int>(
                               state,
                               QString::fromStdString(rightName),
                               QString::fromStdString(leftName)));
            }
            else if (leftType == AttributeDatatype::Float)
            {
                return std::unique_ptr<PreconditionBase>(
                           new IsGreaterPrecondition<float>(
                               state,
                               QString::fromStdString(rightName),
                               QString::fromStdString(leftName)));
            }
        }
    }

    else if (op == OP_AND)
    {
        // remove brackets around term:
        std::string leftTerm = getSubstringByDelimiters(leftSubstring,
                                                        BRACKET_LEFT,
                                                        BRACKET_RIGHT);

        std::string rightTerm = getSubstringByDelimiters(rightSubstring,
                                                         BRACKET_LEFT,
                                                         BRACKET_RIGHT);

        // and generate conditions from terms:
        std::unique_ptr<PreconditionBase> leftCondition =
                stringToPrecondition(state,leftTerm);
        std::unique_ptr<PreconditionBase> rightCondition =
                stringToPrecondition(state, rightTerm);

        // finally generate andCondition
        auto result = std::unique_ptr<ConjunctionPrecondition>(new ConjunctionPrecondition(state));
        result->addCondition(std::move(leftCondition));
        result->addCondition(std::move(rightCondition));
        return std::move(result);
    }
    else if (op == OP_OR)
    {
        // remove brackets around term:
        std::string leftTerm = getSubstringByDelimiters(leftSubstring,
                                                        BRACKET_LEFT,
                                                        BRACKET_RIGHT);

        std::string rightTerm = getSubstringByDelimiters(rightSubstring,
                                                         BRACKET_LEFT,
                                                         BRACKET_RIGHT);

        // and generate conditions from terms:
        std::unique_ptr<PreconditionBase> leftCondition =
                stringToPrecondition(state,leftTerm);
        std::unique_ptr<PreconditionBase> rightCondition =
                stringToPrecondition(state, rightTerm);

        // finally generate andCondition
        auto result = std::unique_ptr<DisjunctionPrecondition>(new DisjunctionPrecondition(state));
        result->addCondition(std::move(leftCondition));
        result->addCondition(std::move(rightCondition));
        return std::move(result);
    }
    else if (op == OP_NOT)
    {
        // remove brackets:
        std::string term = getSubstringByDelimiters(rightSubstring,
                                                    BRACKET_LEFT,
                                                    BRACKET_RIGHT);

        // generate condition from term
        std::unique_ptr<PreconditionBase> rightCondition =
                stringToPrecondition(state, term);

        // make the notCondition
        return std::unique_ptr<PreconditionBase>(
                    new NegationPrecondition(state,
                                          std::move(rightCondition)));
    }

    // DEBUG
    std::cout << "something went wrong..." << std::endl;

    throw std::runtime_error("something went wrong");

}

