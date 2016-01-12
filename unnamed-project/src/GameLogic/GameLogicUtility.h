#ifndef UNNAMED_PROJECT_GAMELOGICUTILITY_H
#define UNNAMED_PROJECT_GAMELOGICUTILITY_H

#include <string>
#include <memory>
#include <utility>
#include "GameLogic/PreconditionBase.h"

#define OP_AND '&'
#define OP_OR '|'
#define OP_NOT '!'
#define OP_EQUALS '='
#define OP_GREATER '>'
#define OP_LESS '<'
#define TYPE_BRACKET_LEFT '['
#define TYPE_BRACKET_RIGHT ']'
#define NAME_BRACKET_LEFT '{'
#define NAME_BRACKET_RIGHT '}'
#define BRACKET_LEFT '('
#define BRACKET_RIGHT ')'

class GlobalState;

namespace gameLogicUtility
{

std::string getSubstringByDelimiters(const std::string &src,
                                     char leftDelimiter,
                                     char rightDelimiter);

std::pair<size_t, char> findOperator(const std::string &src);

std::unique_ptr<PreconditionBase> stringToPrecondition(GlobalState *state,
                                                       const std::string &input);

}

#endif // UNNAMED_PROJECT_GAMELOGICUTILITY_H
