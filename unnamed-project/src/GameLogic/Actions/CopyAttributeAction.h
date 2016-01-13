#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_COPY_ATTRIBUTE_ACTION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_COPY_ATTRIBUTE_ACTION_H

#include "GameLogic/Actions/ActionBase.h"

class CopyAttributeAction : public ActionBase
{
public:
    CopyAttributeAction(GlobalState *state, const QString &src, const QString &dst);
    virtual ~CopyAttributeAction();

    virtual void performAction() override;

    virtual ActionType getActionType() override;

    virtual QString getDataType() override;

    const QString &getSourceKey();
    const QString &getDestKey();

protected:
    QString m_srcKey;
    QString m_dstKey;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_COPY_ATTRIBUTE_ACTION_H
