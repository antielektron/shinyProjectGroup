#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H

#include <QString>

#include "GameLogic/Preconditions/PreconditionBase.h"

class BinaryPreconditionBase : public PreconditionBase
{
public:
    BinaryPreconditionBase(GlobalState *globalState,
                           const QString fieldA,
                           const QString fieldB);
    virtual ~BinaryPreconditionBase();

protected:
    QString m_fieldA;
    QString m_fieldB;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H
