#ifndef UNNAMED_PROJECT_ISGREATERPRECONDITION_H
#define UNNAMED_PROJECT_ISGREATERPRECONDITION_H


#include "GameLogic/BinaryPreconditionBase.h"
#include <QString>

template<class T>
class IsGreaterPrecondition : public BinaryPreconditionBase
{
public:
    IsGreaterPrecondition(GlobalState *state,const QString &objA, const QString &objB);
    virtual ~IsGreaterPrecondition();

    virtual bool evaluateCondition() override;

    virtual QString toQString() override;

protected:

};

#endif // UNNAMED_PROJECT_ISGREATERPRECONDITION_H
