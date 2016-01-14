#ifndef UNNAMED_PROJET_GAME_LOGIC_ACTIONS_ACTION_BASE_H
#define UNNAMED_PROJET_GAME_LOGIC_ACTIONS_ACTION_BASE_H

#include <QString>

enum class ActionType
{
    FlipBoolean,
    CopyAttribute,
    ArithmeticalAction
};

class GlobalState;

class ActionBase
{
public:
    ActionBase(GlobalState *globalState);
    virtual ~ActionBase();

    virtual void performAction() = 0;

    virtual ActionType getActionType() = 0;

    virtual QString getDataType() = 0;

    virtual QString toQString() { return "Empty Action"; } // TODO

protected:
    GlobalState *m_globalState;
};

#endif // UNNAMED_PROJET_GAME_LOGIC_ACTIONS_ACTION_BASE_H
