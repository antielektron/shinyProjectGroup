#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_PRECONDITION_BASE_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_PRECONDITION_BASE_H

#include <QString>

class GlobalState;

class PreconditionBase
{
public:
    PreconditionBase(GlobalState *state);
    virtual ~PreconditionBase();

    virtual bool evaluateCondition() = 0;

    virtual QString toQString() = 0;

protected:
    GlobalState *m_globalState;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_PRECONDITION_BASE_H
