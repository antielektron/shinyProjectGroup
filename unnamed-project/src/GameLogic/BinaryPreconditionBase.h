#ifndef BINARYPRECONDITIONBASE_H
#define BINARYPRECONDITIONBASE_H

#include <QString>
#include "GameLogic/PreconditionBase.h"

class BinaryPreconditionBase : public PreconditionBase
{
public:
    BinaryPreconditionBase(GlobalState *globalState,
                           const QString objA,
                           const QString objB);
    virtual ~BinaryPreconditionBase();

protected:
    QString m_objA;
    QString m_objB;
};

#endif // BINARYPRECONDITIONBASE_H
