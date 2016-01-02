#ifndef UNNAMED_PROJECT_SETATTRIBUTEACTION_H
#define UNNAMED_PROJECT_SETATTRIBUTEACTION_H

#include "GameLogic/ActionBase.h"

class CopyAttributeAction : public ActionBase
{
public:
    CopyAttributeAction(GlobalState *state,
                       const QString &src,
                       const QString &dst);

    virtual ~CopyAttributeAction();

    virtual void performAction() override;

protected:
    QString m_srcKey;
    QString m_dstKey;

};

#endif // UNNAMED_PROJECT_SETATTRIBUTEACTION_H
