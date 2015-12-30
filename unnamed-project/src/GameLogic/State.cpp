#include "GameLogic/State.h"

//------------------------------------------------------------------------------
State::State(std::string name, State *defaultNextState, bool isFinalState)
{
    m_name = name;
    m_defaultNextState = defaultNextState;
    m_isFinalState = isFinalState;
}

//------------------------------------------------------------------------------
State::~State()
{
    // nothing to do here...
}

//------------------------------------------------------------------------------
void State::addTransition(std::string input, State *nextState)
{
    m_stateTransitions[input] = nextState;
}

//------------------------------------------------------------------------------
void State::setEpsilonTransition(State *nextState)
{
    m_defaultNextState = nextState;
}

//------------------------------------------------------------------------------
State *State::getNextState(std::string input)
{
    const auto &it = m_stateTransitions.find(input);
    if (it == m_stateTransitions.end())
    {
        // maybe print a warn message:
        return m_defaultNextState;
    }
    return it->second;
}

//------------------------------------------------------------------------------
State *State::getEpsilonState()
{
    return m_defaultNextState;
}

//------------------------------------------------------------------------------
void State::setFinalState(bool isFinalState)
{
    m_isFinalState = isFinalState;
}

//------------------------------------------------------------------------------
bool State::isFinalState()
{
    return m_isFinalState;
}

//------------------------------------------------------------------------------
range<State::TransitionMapType> State::getTransitions()
{
    return range<State::TransitionMapType>(m_stateTransitions.begin(),
                                           m_stateTransitions.end());
}

//------------------------------------------------------------------------------
const std::string &State::getName()
{
    return m_name;
}

//------------------------------------------------------------------------------
void State::action()
{
    performAction();
}
