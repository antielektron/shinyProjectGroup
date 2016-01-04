#ifndef UNNAMED_PROJECT_ISEQUALPRECONDITION_H
#define UNNAMED_PROJECT_ISEQUALPRECONDITION_H

#include "GameLogic/BinaryPreconditionBase.h"
#include <QString>

template<class T>
class IsEqualPrecondition : public BinaryPreconditionBase
{
public:
    IsEqualPrecondition(GlobalState *state,const QString &objA, const QString &objB);
    virtual ~IsEqualPrecondition();

    virtual bool evaluateCondition() override;

    virtual QString toQString() override;

    virtual QString getTypeofArgs() override;

protected:

};

#endif // UNNAMED_PROJECT_ISEQUALPRECONDITION_H
