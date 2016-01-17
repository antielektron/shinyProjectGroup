#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H

#include <memory>
#include <QDomElement>

#include "GameLogic/Preconditions/PreconditionBase.h"
#include "GameLogic/Expressions/ExpressionConstant.h"

#include "GameLogic/Factories/ExpressionFactory.h"

template <typename T>
class BinaryPreconditionBase : public PreconditionBase
{
public:
    typedef Expression<T> ExpressionType;

    BinaryPreconditionBase();
    BinaryPreconditionBase(GlobalState *state, const QDomElement &domElement);
    BinaryPreconditionBase(std::unique_ptr<ExpressionType> exprA, std::unique_ptr<ExpressionType> exprB);

    virtual ~BinaryPreconditionBase() {}

    virtual QString string() override;

    virtual void writeToXml(QXmlStreamWriter &writer) override;

    inline ExpressionType *getLhs() { return m_exprA.get(); }
    inline ExpressionType *getRhs() { return m_exprB.get(); }

    void setLhs(std::unique_ptr<ExpressionType> expr) { m_exprA = std::move(expr); }
    void setRhs(std::unique_ptr<ExpressionType> expr) { m_exprB = std::move(expr); }

protected:
    std::unique_ptr<ExpressionType> m_exprA;
    std::unique_ptr<ExpressionType> m_exprB;
};


//------------------------------------------------------------------------------
template <typename T>
BinaryPreconditionBase<T>::BinaryPreconditionBase() :
        m_exprA(new ExpressionConstant<T>()),
        m_exprB(new ExpressionConstant<T>())
{}

//------------------------------------------------------------------------------
template <typename T>
BinaryPreconditionBase<T>::BinaryPreconditionBase(GlobalState *state, const QDomElement &domElement)
{
    QString lhs = domElement.attribute("lhs");
    QString rhs = domElement.attribute("rhs");

    m_exprA = Factory::createExpressionFromString<T>(state, lhs);
    m_exprB = Factory::createExpressionFromString<T>(state, rhs);
}

//------------------------------------------------------------------------------
template <typename T>
BinaryPreconditionBase<T>::BinaryPreconditionBase(std::unique_ptr<ExpressionType> exprA, std::unique_ptr<ExpressionType> exprB) :
        m_exprA(std::move(exprA)),
        m_exprB(std::move(exprB))
{}

//------------------------------------------------------------------------------
template <typename T>
QString BinaryPreconditionBase<T>::string()
{
    return m_exprA->string() + " " + this->type() + " " + m_exprB->string();
}

//------------------------------------------------------------------------------
template <typename T>
void BinaryPreconditionBase<T>::writeToXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("Precondition");
    writer.writeAttribute("type", this->type());

    writer.writeAttribute("lhs", m_exprA->string());
    writer.writeAttribute("rhs", m_exprB->string());

    writer.writeEndElement();
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BINARY_PRECONDITION_BASE_H
