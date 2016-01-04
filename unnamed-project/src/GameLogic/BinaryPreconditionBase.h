#ifndef UNNAMED_PROJECT_BINARYPRECONDITIONBASE_H
#define UNNAMED_PROJECT_BINARYPRECONDITIONBASE_H

#include <QString>
#include "GameLogic/PreconditionBase.h"

class BinaryPreconditionBase : public PreconditionBase
{
public:
    BinaryPreconditionBase(GlobalState *globalState,
                           const QString objA,
                           const QString objB);
    virtual ~BinaryPreconditionBase();

    virtual QString getTypeofArgs() = 0;

protected:
    QString m_objA;
    QString m_objB;
};

#endif // UNNAMED_PROJECT_BINARYPRECONDITIONBASE_H
