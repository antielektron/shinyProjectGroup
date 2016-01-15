#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_INVERT_ATTRIBUTE_ACTION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_INVERT_ATTRIBUTE_ACTION_H

#include <QDomElement>

#include "GameLogic/Actions/ActionBase.h"
#include "GameLogic/Actions/Traits.h"
#include "GameLogic/GlobalState.h"

template <typename T>
class InvertAttributeAction : public ActionBase
{
public:
    InvertAttributeAction(GlobalState *state);
    InvertAttributeAction(GlobalState *state, const QString &key);
    InvertAttributeAction(GlobalState *state, const QDomElement &domElement);

    virtual ~InvertAttributeAction() {}

    virtual void performAction() override;

    virtual QString string() override;

    virtual QString type() override;

    virtual void writeToXml(QXmlStreamWriter &writer) override;

    inline const QString &getKey() { return m_key; }

protected:
    GlobalState *m_globalState;
    QString m_key;
};


//------------------------------------------------------------------------------
template <typename T>
InvertAttributeAction<T>::InvertAttributeAction(GlobalState *state) :
    m_globalState(state),
    m_key("empty")
{}

//------------------------------------------------------------------------------
template <typename T>
InvertAttributeAction<T>::InvertAttributeAction(GlobalState *state, const QString &key) :
        m_globalState(state),
        m_key(key)
{}

//------------------------------------------------------------------------------
template <typename T>
InvertAttributeAction<T>::InvertAttributeAction(GlobalState *state, const QDomElement &domElement) :
        m_globalState(state)
{
    m_key = domElement.attribute("key");
}

//------------------------------------------------------------------------------
template <typename T>
void InvertAttributeAction<T>::performAction()
{
    const QVariant &variant = this->m_globalState->getValue(this->m_key);
    T value = variant.value<T>();

    this->m_globalState->setValue(this->m_key, QVariant(-value));
}

//------------------------------------------------------------------------------
template <typename T>
QString InvertAttributeAction<T>::string()
{
    return this->type() + " [" + m_key + "]";
}

//------------------------------------------------------------------------------
template <typename T>
QString InvertAttributeAction<T>::type()
{
    return QString(traits::action_name<InvertAttributeAction<T>>::value);
}

//------------------------------------------------------------------------------
template <typename T>
void InvertAttributeAction<T>::writeToXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("Action");
    writer.writeAttribute("type", this->type());

    writer.writeAttribute("key", m_key);

    writer.writeEndElement();
}


namespace traits
{

    template <>
    struct action_name<InvertAttributeAction<int>>
    {
        static constexpr const char *value = "iinv";
    };

    template <>
    struct action_name<InvertAttributeAction<double>>
    {
        static constexpr const char *value = "finv";
    };

    template <>
    struct action_name<InvertAttributeAction<bool>>
    {
        static constexpr const char *value = "binv";
    };

}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_INVERT_ATTRIBUTE_ACTION_H
