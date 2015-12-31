#ifndef STATE_H
#define STATE_H

#include <map>
#include <string>
#include <QObject>
#include "smartiterator.h"

class State : public QObject
{
    Q_OBJECT
public:
    State(std::string name, State *defaultNextState = nullptr, bool isFinalState = false);
    virtual ~State();

    typedef std::map<std::string, State *>::iterator TransitionMapType;

    void addTransition(std::string input, State *nextState);
    void setEpsilonTransition(State *nextState);
    State *getNextState(std::string input);
    State *getEpsilonState();
    void setFinalState(bool isFinalState);
    bool isFinalState();
    range<TransitionMapType> getTransitions();
    const std::string &getName();

    virtual void performAction();

signals:
    void stateEntered(State *state);
    void stateLeaved(State *state);

public slots:
    void action();

protected:
    std::map<std::string, State *> m_stateTransitions;
    State *m_defaultNextState; //<** for epsilon transition
    bool m_isFinalState;
    std::string m_name;
};

#endif // STATE_H
