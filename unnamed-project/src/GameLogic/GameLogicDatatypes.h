#ifndef UNNAMED_PROJECT_GAME_LOGIC_DATATYPES_H
#define UNNAMED_PROJECT_GAME_LOGIC_DATATYPES_H

#include <map>
#include <QString>
#include "GameLogic/Actions/ActionBase.h"
#include "GameLogic/Animators/Animator.h"

// TODO remove enums and type stuff from ligic classes and move
// it to this header, make Scene io-stuff completely depended
// from this!

#define KEY_ATTRIBUTE_TIME "TIME"
#define KEY_ATTRIBUTE_DELTA_TIME "DELTA_TIME"
#define KEY_ATTRIBUTE_PLAYER "PLAYER_POSITION"
#define KEY_ATTRIBUTE_TRUE "TRUE"
#define KEY_ATTRIBUTE_FALSE "FALSE"

enum class ArithmeticalOperationType
{
    additionType,
    subtractionType,
    multiplicationType,
    divisionType
};

enum class AttributeDatatype
{
    Bool,
    Int,
    Float,
    QVector3D
};

const std::map<AttributeDatatype, QString> typeToQString =
{
    { AttributeDatatype::Bool, "bool" },
    { AttributeDatatype::Int, "int" },
    { AttributeDatatype::Float, "float" },
    { AttributeDatatype::QVector3D, "vector" }
};

const std::map<QString, AttributeDatatype> qStringToType =
{
    { "bool", AttributeDatatype::Bool },
    { "int", AttributeDatatype::Int },
    { "float", AttributeDatatype::Float },
    { "vector", AttributeDatatype::QVector3D }
};

const std::map<ActionType, QString> actionToQString =
{
    { ActionType::ArithmeticalAction, "arithmetical" },
    { ActionType::CopyAttribute, "copy" },
    { ActionType::FlipBoolean, "flip" }
};

const std::map<QString, ActionType> qStringToAction =
{
    { "arithmetical", ActionType::ArithmeticalAction },
    { "copy", ActionType::CopyAttribute },
    { "flip", ActionType::FlipBoolean }
};

const std::map<AnimationType, QString> animationToQString =
{
    { AnimationType::Position, "position" },
    { AnimationType::Rotation, "rotation" },
    { AnimationType::Scaling, "scaling" }
};

const std::map<QString, AnimationType> qStringToAnimation =
{
    { "position", AnimationType::Position },
    { "rotation", AnimationType::Rotation },
    { "scaling", AnimationType::Scaling }
};

const std::map<QString, InterpolationType> qStringToInterpolation =
{
    { "jump", InterpolationType::Jump },
    { "linear", InterpolationType::Linear },
    { "trigonometric", InterpolationType::Trigonometric }
};

const std::map<InterpolationType, QString> interpolationToQString =
{
    { InterpolationType::Jump, "jump" },
    { InterpolationType::Linear, "linear" },
    { InterpolationType::Trigonometric, "trigonometric" }
};

const std::map<QString, ArithmeticalOperationType> qStringToArithOperationType =
{
    {"+", ArithmeticalOperationType::additionType},
    {"-", ArithmeticalOperationType::subtractionType},
    {"*", ArithmeticalOperationType::multiplicationType},
    {"/", ArithmeticalOperationType::divisionType}
};

const std::map<ArithmeticalOperationType, QString> arithOperationTypeToQString =
{
    {ArithmeticalOperationType::additionType, "+"},
    {ArithmeticalOperationType::subtractionType, "-"},
    {ArithmeticalOperationType::multiplicationType, "*"},
    {ArithmeticalOperationType::divisionType, "/"}
};





#endif //UNNAMED_PROJECT_GAME_LOGIC_DATATYPES_H
