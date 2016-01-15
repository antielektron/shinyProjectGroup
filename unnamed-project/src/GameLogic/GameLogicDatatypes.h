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
#define KEY_ATTRIBUTE_PLAYER "PLAYER_POSITION"

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

#endif // UNNAMED_PROJECT_GAME_LOGIC_DATATYPES_H
