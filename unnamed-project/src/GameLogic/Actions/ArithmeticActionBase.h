#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_ARITHMETICAL_ACTION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_ARITHMETICAL_ACTION_H

#include <memory>
#include <QDomElement>

#include "GameLogic/Actions/ActionBase.h"
#include "GameLogic/Expressions/ExpressionConstant.h"

#include "GameLogic/Factories/ExpressionFactory.h"

template <typename T>
class ArithmeticActionBase : public ActionBase
{
public:
    typedef Expression<T> ExpressionType;

    // Allow empty initialization
    ArithmeticActionBase(GlobalState *state);
    ArithmeticActionBase(GlobalState *state, const QDomElement &domElement);
    ArithmeticActionBase(GlobalState *state, const QString &key, std::unique_ptr<ExpressionType> expr);

    virtual ~ArithmeticActionBase() {}

    virtual QString string() override;

    virtual void writeToXml(QXmlStreamWriter &writer) override;

    inline const QString &getLhs() { return m_lhsKey; }
    inline Expression<T> *getRhs() { return m_rhsExpr.get(); }

    void setLhs(const QString &key) { m_lhsKey = key; }
    void setRhs(std::unique_ptr<ExpressionType> expr) { m_rhsExpr = std::move(expr); }

protected:
    GlobalState *m_globalState;
    QString m_lhsKey;
    std::unique_ptr<ExpressionType> m_rhsExpr;
};


//------------------------------------------------------------------------------
template <typename T>
ArithmeticActionBase<T>::ArithmeticActionBase(GlobalState *state) :
        m_lhsKey("empty"),
        m_rhsExpr(new ExpressionConstant<T>()),
        m_globalState(state)
{}

//------------------------------------------------------------------------------
template <typename T>
ArithmeticActionBase<T>::ArithmeticActionBase(GlobalState *state, const QDomElement &domElement) :
        m_globalState(state)
{
    m_lhsKey = domElement.attribute("lhs");

    QString rhs = domElement.attribute("rhs");
    m_rhsExpr = Factory::createExpressionFromString<T>(state, rhs);
}

//------------------------------------------------------------------------------
template <typename T>
ArithmeticActionBase<T>::ArithmeticActionBase(GlobalState *state, const QString &key, std::unique_ptr<ExpressionType> expr) :
        m_lhsKey(key),
        m_rhsExpr(std::move(expr))
{}

//------------------------------------------------------------------------------
template <typename T>
QString ArithmeticActionBase<T>::string()
{
    return "[" + m_lhsKey + "] " + this->type() + " " + m_rhsExpr->string();
}

//------------------------------------------------------------------------------
template <typename T>
void ArithmeticActionBase<T>::writeToXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("Action");
    writer.writeAttribute("type", this->type());

    writer.writeAttribute("lhs", m_lhsKey);
    writer.writeAttribute("rhs", m_rhsExpr->string());

    writer.writeEndElement();
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_ARITHMETICAL_ACTION_H
