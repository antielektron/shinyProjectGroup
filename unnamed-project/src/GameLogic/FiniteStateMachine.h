#ifndef UNNAMED_PROJECT_FINITESTATEMACHINE_H
#define UNNAMED_PROJECT_FINITESTATEMACHINE_H

#include <QObject>
#include <memory>
#include <map>
#include "GameLogic/State.h"

class FiniteStateMachine
{
    Q_OBJECT
public:
    FiniteStateMachine();
    FiniteStateMachine(std::unique_ptr<State> startState);
    virtual ~FiniteStateMachine();

    State *addState(std::unique_ptr<State> state);
    void addTransition(std::string srcStateName,
                       std::string dstStateName,
                       std::string inputName);
    void addEpsilonTransition(std::string srcStateName,
                              std::string dstStateName);

protected:
    State                                           *m_startState;
    std::map<std::string, std::unique_ptr<State>>   m_stateMap;

    State                                           *currentState;
};

#endif // UNNAMED_PROJECT_FINITESTATEMACHINE_H
