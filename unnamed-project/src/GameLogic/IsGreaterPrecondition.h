#ifndef ISGREATERPRECONDITION_H
#define ISGREATERPRECONDITION_H


#include "GameLogic/BinaryPreconditionBase.h"
#include <QString>

template<class T>
class IsGreaterPrecondition : public BinaryPreconditionBase
{
public:
    IsGreaterPrecondition(GlobalState *state,const QString &objA, const QString &objB);
    virtual ~IsGreaterPrecondition();

    virtual bool evaluateCondition() override;

protected:

};

#endif // ISGREATERPRECONDITION_H
