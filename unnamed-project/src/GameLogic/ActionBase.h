#ifndef UNNAMED_PROJET_ACTIONBASE_H
#define UNNAMED_PROJET_ACTIONBASE_H

class GlobalState;

class ActionBase
{
public:
    ActionBase(GlobalState *globalState);
    virtual ~ActionBase();

    virtual void performAction() = 0;

protected:

    GlobalState *m_globalState;

};

#endif // UNNAMED_PROJET_ACTIONBASE_H
