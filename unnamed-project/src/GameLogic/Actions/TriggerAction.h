#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_TRIGGER_ACTION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_TRIGGER_ACTION_H

#include <memory>
#include <QDomElement>
#include <QXmlStreamWriter>

#include "GameLogic/Actions/ActionBase.h"
#include "GameLogic/Traits.h"

template <typename T>
class Expression;

class GlobalState;
class Scene;

class TriggerAction : public ActionBase
{
public:
    TriggerAction(GlobalState *state);
    TriggerAction(GlobalState *state, const QDomElement &domElement);

    virtual void performAction() override;

    virtual QString string() override;

    virtual QString type() override;

    virtual void writeToXml(QXmlStreamWriter &writer) override;

    void setDelay(std::unique_ptr<Expression<double>> delay);

    Expression<double> *getDelay();

    void setName(const QString &name);

    const QString &getName();

private:
    Scene *m_scene;

    QString m_name;

    std::unique_ptr<Expression<double>> m_delay;
};


namespace traits
{
    template <>
    struct action_name<TriggerAction>
    {
        static const char *value() { return "trigger"; }
    };
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_TRIGGER_ACTION_H
