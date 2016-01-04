#include "FiniteStateMachine.h"

//------------------------------------------------------------------------------
FiniteStateMachine::FiniteStateMachine() :
    FiniteStateMachine(std::unique_ptr<State>(new State("start")))
{

}

//------------------------------------------------------------------------------
FiniteStateMachine::FiniteStateMachine(std::unique_ptr<State> startState)
{
    m_stateMap[startState->getName()] = std::move(startState);
    m_startState = startState.get();
}

//------------------------------------------------------------------------------
FiniteStateMachine::~FiniteStateMachine()
{
    // nothing to do here
}

//------------------------------------------------------------------------------
State *FiniteStateMachine::addState(std::unique_ptr<State> state)
{
    m_stateMap[state->getName()] = std::move(state);
    return 0;
}

//------------------------------------------------------------------------------
void FiniteStateMachine::addTransition(std::string srcStateName,
                                       std::string dstStateName,
                                       std::string inputName)
{
    m_stateMap[srcStateName]->addTransition(inputName,
                                            m_stateMap[dstStateName].get());
}

//------------------------------------------------------------------------------
void FiniteStateMachine::addEpsilonTransition(std::string srcStateName,
                                              std::string dstStateName)
{
    m_stateMap[srcStateName]->setEpsilonTransition(m_stateMap[dstStateName].get());
}
