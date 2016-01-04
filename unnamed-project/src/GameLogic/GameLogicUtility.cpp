#include "GameLogic/GameLogicUtility.h"
#include "GameLogic/IsAndPrecondition.h"
#include "GameLogic/IsNotPrecondition.h"
#include "GameLogic/IsEqualPrecondition.h"
#include "GameLogic/IsGreaterPrecondition.h"
#include "GameLogic/GlobalState.h"

#include <cassert>

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
        return src.substr(indexRightBracket + 1,
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
    }

    std::string leftSubstring = input.substr(0, opIndex);
    std::string rightSubstring = input.substr(opIndex + 1,
                                              input.length() - opIndex - 1);

    // DEBUG
    std::cout << "source String: "
              << input << std::endl;

    if (op == OP_EQUALS || op == OP_GREATER)
    {
        // in this case we have to determine the type
        // and values from the substrings

        std::string leftType = getSubstringByDelimiters(leftSubstring,
                                                        TYPE_BRACKET_LEFT,
                                                        TYPE_BRACKET_RIGHT);

        std::string rightType = getSubstringByDelimiters(rightSubstring,
                                                         TYPE_BRACKET_LEFT,
                                                         TYPE_BRACKET_RIGHT);

        std::string leftName = getSubstringByDelimiters(leftSubstring,
                                                        NAME_BRACKET_LEFT,
                                                        NAME_BRACKET_RIGHT);

        std::string rightName = getSubstringByDelimiters(rightSubstring,
                                                         NAME_BRACKET_LEFT,
                                                         NAME_BRACKET_RIGHT);

        // DEBUG
        std::cout << "leftType: "       << leftType
                  << "\nrightType: "    << rightType
                  << "\nleftName: "     << leftName
                  << "\nrightName: "    << rightName
                  << std::endl;

        assert(leftType == rightType);

        if (op == OP_EQUALS)
        {
            if (leftType == "int")
            {
                return std::unique_ptr<PreconditionBase>(
                           new IsEqualPrecondition<int>(
                               state,
                               QString::fromStdString(leftName),
                               QString::fromStdString(rightName)));
            }
            else if (leftType == "bool")
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
            if (leftType == "int")
            {
                return std::unique_ptr<PreconditionBase>(
                           new IsGreaterPrecondition<int>(
                               state,
                               QString::fromStdString(leftName),
                               QString::fromStdString(rightName)));
            }
            else if (leftType == "float")
            {
                return std::unique_ptr<PreconditionBase>(
                           new IsGreaterPrecondition<float>(
                               state,
                               QString::fromStdString(leftName),
                               QString::fromStdString(rightName)));
            }
        }
    }

    else if (op == OP_AND)
    {
        std::unique_ptr<PreconditionBase> leftCondition =
                stringToPrecondition(state,leftSubstring);
        std::unique_ptr<PreconditionBase> rightCondition =
                stringToPrecondition(state, rightSubstring);

        assert(leftCondition && rightCondition);

        return std::unique_ptr<PreconditionBase>(
                    new IsAndPrecondition(state,
                                          std::move(leftCondition),
                                          std::move(rightCondition)));
    }
    else if (op == OP_NOT)
    {
        std::unique_ptr<PreconditionBase> rightCondition =
                stringToPrecondition(state, rightSubstring);

        assert(rightCondition);

        return std::unique_ptr<PreconditionBase>(
                    new IsNotPrecondition(state,
                                          std::move(rightCondition)));
    }

    // DEBUG
    std::cout << "something went wrong..." << std::endl;

    return std::unique_ptr<PreconditionBase>(nullptr);

}

