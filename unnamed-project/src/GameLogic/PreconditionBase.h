#ifndef UNNAMED_PROJECT_PRECONDITIONBASE_H
#define UNNAMED_PROJECT_PRECONDITIONBASE_H

class GlobalState;

class PreconditionBase
{
public:
    PreconditionBase(GlobalState *state);
    virtual ~PreconditionBase();

    virtual bool evaluateCondition() = 0;

protected:
    GlobalState *m_globalState;
};

#endif // UNNAMED_PROJECT_PRECONDITIONBASE_H
