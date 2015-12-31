#ifndef UNNAMED_PROJECT_STATETRANSITION_H
#define UNNAMED_PROJECT_STATETRANSITION_H

#include <string>

class StateTransition
{
public:
    StateTransition(const std::string &name);
    virtual ~StateTransition();

    const std::string &getName();

protected:
    std::string m_eventName;
};

#endif // UNNAMED_PROJECT_STATETRANSITION_H
