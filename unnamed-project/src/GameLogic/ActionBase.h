#ifndef UNNAMED_PROJET_ACTIONBASE_H
#define UNNAMED_PROJET_ACTIONBASE_H

enum class ActionType
{
    FlipBoolean,
    CopyAttribute,
    ArithmeticalAction
};

class GlobalState;
class QString;

class ActionBase
{
public:
    ActionBase(GlobalState *globalState);
    virtual ~ActionBase();

    virtual void performAction() = 0;

    virtual ActionType getActionType() = 0;

    virtual QString getDataType() = 0;

protected:

    GlobalState *m_globalState;

};

#endif // UNNAMED_PROJET_ACTIONBASE_H
